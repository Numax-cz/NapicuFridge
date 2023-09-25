
/**
 * @file main.cpp
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-27-05
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
*/



// Připojení potřebných lokálních knihoven z /src/include
#include <include/main.h>
//Proměnná pro ukládání zda je zařízení připojené
bool devicePaired = false;


//Proměnná pro ukládání zda je resetovací dioda aktivní
bool resetLEDOn = false;
//Proměnná aktuální doby u 
unsigned long reset_led_time_now = 0;

int resetLEDBlinkCount = 0;

fridge_data FridgeData;

// Inicializace modulu z knihovny

BLEServer* pServer = NULL;

FridgeTempDHT* insideTempDHT = NULL;

FridgeTempDHT* outsideTempDHT = NULL;

ButtonManager* resetButton = NULL;

RelayModule* test_relay_1 = NULL;

Thermistor* thermistor;

DigiPot* digitalPotentiometer = NULL;

//Proměnná doby, po kterou se má čekat mezi komunikací s bluetooth
const int data_send_period = 1000;
//Proměnná aktuální doby v komunikací s bluetooth
unsigned long data_send_time_now = 0;

void my_gap_event_handler(esp_gap_ble_cb_event_t  event, esp_ble_gap_cb_param_t* param) {
  switch(event) {
      // Proveď následující po ověření zařízení
      case ESP_GAP_BLE_AUTH_CMPL_EVT: {
        if(param->ble_security.auth_cmpl.success) {
          // Vypsání zprávy do konzole
          Serial.println("Paired");
          //Pokud zařízení nebylo nikdy spárované s žádným zařízením
          if(FridgeData.paired_device_address == nullptr){
            //Získání adresy spárovaného zařízení a vytvoření classy BLEAddress
            BLEAddress* address = new BLEAddress(param->ble_security.auth_cmpl.bd_addr);
            //Uložení proměnné spárovaného zařízení do globálního nastavení chytré ledničky
            FridgeData.paired_device_address = address;
            //Získání reprezentační MAC adresy.
            esp_bd_addr_t* address_native = address->getNative();

            for (int i = 0; i < MAC_EEPROM_SIZE; ++i) {
              //Zapsání hodnoty do EEPROM
              EEPROM.write(MAC_ADDRESS_EEPROM_ADDR + i, (*address_native)[i]);
            }
            //Potvrzení změn
            EEPROM.commit();
          } 


           
          if(FridgeDisplay::get_display_state() == FRIDGE_DISPLAY_PAIR_TEXT) {
            FridgeDisplay::change_display_state(FRIDGE_DISPLAY_IN_TEMP_1);
          }


          //Nastavení proměnné na log1
          devicePaired = true;
        } else {
          //Odpojení zařízení v případě neúspěšného ověření
          esp_ble_gap_disconnect(param->ble_security.auth_cmpl.bd_addr);
        }
        break;

    }   
  }
}



void setup() {
  // Zahájení komunikace po sériové lince
  // Rychlostí 9600 baud
  Serial.begin(9600);


  //Nastavení LED diod jako výstup
  pinMode(CONNECTION_LED, OUTPUT);
  pinMode(RESET_LED, OUTPUT);



  //Vytvoření třídy pro tlačítko 
  resetButton = new ButtonManager(RESET_BUTTON);
  //Spuštění funkce begin
  resetButton->begin();

  //Vytvoření třídy pro relé modul s pinem 25
  test_relay_1 = new RelayModule(25);
  //Spuštění funkce begin
  test_relay_1->begin();

  //Vytvoření třídy pro digitální potenciometr
  digitalPotentiometer = new DigiPot(X9_INC, X9_UD, X9_CS);


  thermistor = new NTC_Thermistor(
    SENSOR_PIN,
    REFERENCE_RESISTANCE,
    NOMINAL_RESISTANCE,
    NOMINAL_TEMPERATURE,
    B_VALUE,
    STM32_ANALOG_RESOLUTION // <- for a thermistor calibration
  );

  //Inicializace paměti EEPROM
  if (!EEPROM.begin(EEPROM_MAX_SIZE)) {
    //Vypsání hodnoty do konzole
    Serial.println("EEPROM inicializace byla neúspěšná");
    while (true);
  } else {
    //Vypsání hodnoty do konzole
    Serial.println("EEPROM je dostupné");
  }

  FridgeDisplay::begin();




  BLEAddress* data_from_eeprom = read_paired_device_mac_address_from_eeprom();

  //Když je adresa uložená v paměti EEPROM provede se následující
  if(data_from_eeprom) {
    //Vypsání hodnoty do konzole
    Serial.println("MAC adresa je uložená v EEPROM.");
    //Vypsání hodnoty z EEPROM do konzole
    Serial.println(data_from_eeprom->toString().c_str());  
    //Nastavení proměnné
    FridgeData.paired_device_address = data_from_eeprom;
    //Změna displeje
    FridgeDisplay::change_display_state(FRIDGE_DISPLAY_IN_TEMP_1);
  } else {
    //Změna displeje
    FridgeDisplay::change_display_state(FRIDGE_DISPLAY_PAIR_TEXT);
  }







  // Inicializace Bluetooth s nastavením jména zařízení
  BLEDevice::init("NapicuFridge");
  // BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);

  BLEDevice::setCustomGapHandler(my_gap_event_handler);


  // Vytvoření BLE serveru
  pServer = BLEDevice::createServer();






  pServer->setCallbacks(new ServerCallBack());
  // Vytvoření BLE služby
  BLEService *pService = pServer->createService(SERVICE_UUID);



  // Vytvoření teploměru pro vnitřní zaznamenávání teploty
  insideTempDHT = new FridgeTempDHT(DHT_INSIDE, CHARACTERISTIC_DHT_INSIDE_UUID, pService, FridgeData.in_temp);
  //Spuštění begin funkce
  insideTempDHT->begin();

  // Vytvoření teploměru pro venkovní zaznamenávání teploty
  outsideTempDHT = new FridgeTempDHT(DHT_OUTSIDE, CHARACTERISTIC_DHT_OUTSIDE_UUID, pService, FridgeData.out_temp);
  //Spuštění begin funkce
  outsideTempDHT->begin();
  

  
  // Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *fridgeEnableCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_DISPLAY_ENABLE_UUID,
    BLECharacteristic::PROPERTY_WRITE | 
    BLECharacteristic::PROPERTY_READ
  );
                                       
  fridgeEnableCharacteristic->setCallbacks(new DisplayEnableCharacteristicCallback());


  // Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *fridgeStateCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_DISPLAY_STATE_UUID,
    BLECharacteristic::PROPERTY_WRITE | 
    BLECharacteristic::PROPERTY_READ
  );
                                       
  fridgeStateCharacteristic->setCallbacks(new DisplayStateCharacteristicCallback());


  




  // Zahájení BLE služby
  pService->start();


  esp_ble_gap_config_local_privacy(true);

  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
  pSecurity->setCapability(ESP_IO_CAP_NONE);
  pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);




  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); 
  pAdvertising->setMinPreferred(0x12);
 
  // Zapnutí viditelnosti BLE 
  BLEDevice::startAdvertising();


  Serial.println("BLE nastaveno, ceka na pripojeni..");
}

void loop() {
  const unsigned long time = millis();



  //Spuštění loop funkce displeje
  FridgeDisplay::loop();


  //Spuštění loop funkce tlačítka
  resetButton->loop();
  //Spuštění funkce pro správu držení tlačítka
  resetButton->button_hold_time(5000, [](){
    //Po podržení tlačítka po dobu 5000ms se provede následující
    //Spuštění funkce pro uvedení zařízení do továního nastavení
    factory_reset();
  });

  //Načasování programu
  if(time >= data_send_time_now + data_send_period) {
    const double celsius = thermistor->readCelsius();

    Serial.print(celsius);
    Serial.print(" C, ");

    data_send_time_now += data_send_period;
    //Aktualizování hodnot 
    insideTempDHT->updateTemperature();
    //Aktualizování hodnot 
    outsideTempDHT->updateTemperature();
    //Pokud je zařízení připojeno k ESP32
    if (devicePaired == true) {
      //Začneme s odesíláním dat
      insideTempDHT->sendTemperature();
      //Začneme s odesíláním dat
      outsideTempDHT->sendTemperature();
    }
  }

  //Blok kódu pro správu resetovacího tlačítka a diody
  //Pookud je proměnná resetLEDOn log1 a zároveň resetLEDBlinkCount menší, nebo rovno 3 provede se následující
  if(resetLEDOn && resetLEDBlinkCount <= 3) {
    //Načasování programu, každých 250ms se provede následující
    if(time >= reset_led_time_now + 250) {
      //Přičte se 250 k proměnné určující aktuální dobu
      reset_led_time_now += 250;
      //Přičte se 1 k proměnné resetLEDBlinkCount
      resetLEDBlinkCount++;
      //Deklarace a uložení hodnoty z reset led pinu
      int pin_value = digitalRead(RESET_LED);
      //Zapsání log1, nebo log0 dle následující podmínky do reset led pinu
      digitalWrite(RESET_LED, !pin_value ? 1 : 0);
      //Pokud je po přičtení 1 resetLEDBlinkCount větší než 4 provede se následující
      if(resetLEDBlinkCount > 3) {
        //Restartování ESP32
        ESP.restart();
      }
    }
  } else {
    //Nastavení proměnné určující aktuální období
    reset_led_time_now = time;
  }
}

BLEAddress* read_paired_device_mac_address_from_eeprom() {
  esp_bd_addr_t data_from_eeprom;
  //Přečtení hodnoty MAC adresy z EEPROM
  for (int i = MAC_ADDRESS_EEPROM_ADDR; i < MAC_EEPROM_SIZE; ++i) {
    data_from_eeprom[i] = EEPROM.read(MAC_ADDRESS_EEPROM_ADDR + i);
    //Vypsání hodnoty do konzole
    Serial.println(data_from_eeprom[i]);
  }

  //Pokud je uložená MAC adresa proveď následující
  if(data_from_eeprom[0] != 0xFF) {
    return new BLEAddress(data_from_eeprom);
  }

  //Vrácení null ukazatele
  return nullptr;
}


//Funkce, která uvede zařízení do továrního nastavení
void factory_reset() {
  //Vypsání hodnoty do konzole
  Serial.println("Tovární nastavení...");
  //Následně vymaževe veškeré data z EEPROM (nastavíme veškeré adresy, které využíváme na 0xFF - 255)
  for(int i = 0; i < EEPROM_MAX_SIZE; i++) {
    //Zapsání hodnoty do EEPROM
    EEPROM.write(i, 0xFF);
  }
  //Potvrzení změn
  EEPROM.commit();
  //Nastavení proměnné na log1
  resetLEDOn = true;
}


