
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

//Připojení hlavní knohovny 
#include <include/main.h>
//Proměnná pro ukládání zda je zařízení připojené
bool devicePaired = false;


fridge_data FridgeData;

// Inicializace modulu z knihovny

//Proměnná pro uložení BLE serveru
BLEServer* pServer = NULL;
//Proměnná pro uložení DHT senzoru vnitřní teploty
FridgeTempDHT* insideTempDHT = NULL;
//Proměnná pro uložení DHT senzoru venkovní teploty
FridgeTempDHT* outsideTempDHT = NULL;
//Proměnná pro uložení třídy resetovacího tlačítka
ButtonManager* resetButton = NULL;
//Proměnná pro uložení třídy relé chladících ventilátorů
RelayModule* relay_cooling_fans = NULL;
//Proměnná pro uložení třídy relé vnitřních ventilátorů
RelayModule* relay_in_fans = NULL;
//Proměnná pro uložení třídy relé hlavního napájení peltierů
RelayModule* relay_peltier = NULL;
//Proměnná pro uložení třídy relé ovládací režim napájení peltierů
RelayModule* relay_peltier_power_mode = NULL;
//Proměnná pro uložení třídy termistoru pro zaznamenávaní teploty teplé strany chladiče
Thermistor* out_thermistor;
//Proměnná pro uložení třídy digitálního potenciometru
DigiPot* digitalPotentiometer = NULL;
//Proměnná pro uložení venkovních chladících PWM ventilátorů
FanController<COOLING_FAN_PWM, COOLING_FAN_TACH> cooling_fans_pwm;
//Proměnná pro uložení třídy správce napájení
PowerManager* fridge_power_manager = NULL;

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


//Setup funkce, která se spustí po zapnutí
void setup() {
  // Zahájení komunikace po sériové lince
  // Rychlostí 9600 baud
  Serial.begin(9600);


  //Nastavení LED diod jako výstup
  pinMode(CONNECTION_LED, OUTPUT);
  //Nastavení piezo pinu jako výstup
  pinMode(PIEZO_PIN, OUTPUT);


    //Inicializace paměti EEPROM
  if (!EEPROM.begin(EEPROM_MAX_SIZE)) {
    //Vypsání hodnoty do konzole
    Serial.println("EEPROM inicializace byla neúspěšná");
    while (true);
  } else {
    //Vypsání hodnoty do konzole
    Serial.println("EEPROM je dostupné");
  }


  //Vytvoření třídy pro tlačítko 
  resetButton = new ButtonManager(RESET_BUTTON);
  //Spuštění funkce begin
  resetButton->begin();

  //Vytvoření třídy pro relé ovládací chladících PWM ventilátorů
  relay_cooling_fans = new RelayModule(RELAY_PWM_FANS_MODULE_PIN);
  //Spuštění funkce begin
  relay_cooling_fans->begin();

  //Vytvoření třídy pro relé ovládací vnitřních ventilátorů
  relay_in_fans = new RelayModule(RELAY_IN_FANS_MODULE_PIN);
  //Spuštění funkce begin
  relay_in_fans->begin();

  //Vytvoření třídy pro relé ovládací hlavní napájení peltierů 
  relay_peltier = new RelayModule(RELAY_PELTIER_PIN);
  //Spuštění funkce begin
  relay_peltier->begin();

  //Vytvoření třídy pro relé ovládací napájecí režim peltierů
  relay_peltier_power_mode = new RelayModule(RELAY_PELTIER_POWER_MODE_PIN);
  //Spuštění funkce begin
  relay_peltier_power_mode->begin();


  //Vytvoření třídy pro digitální potenciometr
  digitalPotentiometer = new DigiPot(X9_INC, X9_UD, X9_CS);



  //Vytvoření třídy pro ntc termistor 
  out_thermistor = new NTC_Thermistor(
    SENSOR_PIN,
    REFERENCE_RESISTANCE,
    NOMINAL_RESISTANCE,
    NOMINAL_TEMPERATURE,
    B_VALUE,
    STM32_ANALOG_RESOLUTION // <- for a thermistor calibration
  );



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

  // Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *fridgeInFansCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_IN_FANS_UUID,
    BLECharacteristic::PROPERTY_WRITE | 
    BLECharacteristic::PROPERTY_READ
  );
                                       
  fridgeInFansCharacteristic->setCallbacks(new InFansCharacteristicCallback());

  // Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *powerModeCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_POWER_MODE_UUID,
    BLECharacteristic::PROPERTY_WRITE | 
    BLECharacteristic::PROPERTY_READ
  );
                                       
  powerModeCharacteristic->setCallbacks(new PowerManagerCharacteristicCallback());


  
  PowerManager::begin();



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

int per = 0;

void loop() {
  //Uložíme aktuální čas běhu do konstantní proměnné time 
  const unsigned long time = millis();



  //Spuštění loop funkce displeje
  FridgeDisplay::loop();

  //Spuštění loop funkce error kontroly
  ErrorChecker::loop();

  //Spuštění loop funkce piezo manageru 
  PiezoManager::loop();



  //Spuštění loop funkce ventilátoru
  // cooling_fans_pwm.loop();  
  

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
    //Získání dat od venkovního termistoru ve stupních celsia
    const double celsius = out_thermistor->readCelsius();

    //Pokud se proměnná celsius rovná -273.15 (nesmyslná hodnota při nezískání dat např. vadný senzor) provede se následující
    if(celsius == -273.15) {
      
    }

 

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

  //Spuštění funkce pro pípnutí piezo
  PiezoManager::tone_beep(2, []() {
    //Po ukončení pípání piezo se provede následující
    //Restartování ESP32
    ESP.restart();
  });
}


