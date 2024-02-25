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

//Připojení hlavní knihovny 
#include <include/main.h>
//Proměnná pro ukládání zda je zařízení připojené
bool devicePaired = false;


fridge_data FridgeData;

// Inicializace modulu z knihovny

//Proměnná pro uložení BLE serveru
BLEServer* pServer = NULL;
//Proměnná pro uložení DHT senzoru vnitřní teploty
FridgeTempDHT* inside_temp_dht = NULL;
//Proměnná pro uložení DHT senzoru venkovní teploty
FridgeTempDHT* outside_temp_dht = NULL;
//Proměnná pro uložení NTC senzoru teploty chladiče
ThermistorManager* cooler_temp_ntc = NULL;
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
//Proměnná pro uložení třídy digitálního potenciometru
DigitalPotentiometer* digital_potentiometer = NULL;
//Proměnná pro uložení venkovních chladících PWM ventilátorů
FanController<COOLING_FAN_PWM, COOLING_FAN_TACH> cooling_fans_pwm;
//Proměnná pro uložení třídy správce napájení
PowerManager* fridge_power_manager = NULL;
//Proměnná pro uložení třídy RGB světla
RGBManager* fridge_rgb = NULL;

//Proměnná doby, po kterou se má čekat mezi komunikací s bluetooth
const int data_send_period = 1000;
//Proměnná aktuální doby v komunikací s bluetooth
unsigned long data_send_time_now = 0;

void ble_gap_event_handler(esp_gap_ble_cb_event_t  event, esp_ble_gap_cb_param_t* param) {
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

          //Spuštění funkce pro nastavení displeje 
          FridgeDisplay::load_display_state_from_eeprom();
          
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

  //Nastavení piezo pinu jako výstup
  pinMode(PIEZO_PIN, OUTPUT);
  //Nastavení pinu dveří jako vstup 
  pinMode(DOOR_PIN, INPUT);

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
  digital_potentiometer = new DigitalPotentiometer(X9_INC, X9_UD, X9_CS);

  //Spuštění begin funkce displeje 
  FridgeDisplay::begin();

  //Uložení a získání uložené mac adresy
  BLEAddress* data_from_eeprom = read_paired_device_mac_address_from_eeprom();

  //Když je adresa uložená v paměti EEPROM provede se následující
  if(data_from_eeprom) {
    //Pokud je zapnutý vývojářký režim, provede se následující 
    if(DEV_MODE) {
      //Vypsání hodnoty do konzole
      Serial.println("MAC adresa je uložená v EEPROM.");
      //Vypsání hodnoty z EEPROM do konzole
      Serial.println(data_from_eeprom->toString().c_str());  
    }
    //Nastavení proměnné
    FridgeData.paired_device_address = data_from_eeprom;
    //Spuštění funkce pro nastavení displeje
    FridgeDisplay::load_display_state_from_eeprom();
  } else {
    //Změna displeje
    FridgeDisplay::change_display_state(FRIDGE_DISPLAY_PAIR_TEXT);
  }

  // Inicializace Bluetooth s nastavením jména zařízení
  BLEDevice::init(DEFAULT_BLE_NAME);
  // BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
  //Nastavení vlastní obslužné funkce událostí pro Bluetooth GAP
  BLEDevice::setCustomGapHandler(ble_gap_event_handler);

  // Vytvoření BLE serveru
  pServer = BLEDevice::createServer();

  //Nastavení zpětného volání pro server
  pServer->setCallbacks(new ServerCallBack());
  // Vytvoření BLE služby
  BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID), 50, 0);

  // Vytvoření teploměru pro vnitřní zaznamenávání teploty
  inside_temp_dht = new FridgeTempDHT(DHT_INSIDE, CHARACTERISTIC_DHT_INSIDE_UUID, pService, FridgeData.in_temp);
  //Spuštění begin funkce
  inside_temp_dht->begin();

  // Vytvoření teploměru pro venkovní zaznamenávání teploty
  outside_temp_dht = new FridgeTempDHT(DHT_OUTSIDE, CHARACTERISTIC_DHT_OUTSIDE_UUID, pService, FridgeData.out_temp);
  //Spuštění begin funkce
  outside_temp_dht->begin();

  //Vytvoření třídy pro ntc termistor 
  cooler_temp_ntc = new ThermistorManager(
    COOLER_NTC_SENSOR_PIN,
    CHARACTERISTIC_NTC_COOLER_UUID,
    pService,
    FridgeData.cooler_temp,
    COOLER_NTC_REFERENCE_RESISTANCE,
    COOLER_NTC_NOMINAL_RESISTANCE,
    COOLER_NTC_NOMINAL_TEMPERATURE,
    COOLER_NTC_B_VALUE,
    COOLER_NTC_STM32_ANALOG_RESOLUTION
  );
  
  //Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *fridgeEnableCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_DISPLAY_ENABLE_UUID,
    BLECharacteristic::PROPERTY_WRITE | 
    BLECharacteristic::PROPERTY_READ
  );

  //Nastavení zpětného volání pro danou charakteristiku                      
  fridgeEnableCharacteristic->setCallbacks(new DisplayEnableCharacteristicCallback());

  //Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *fridgeStateCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_DISPLAY_STATE_UUID,
    BLECharacteristic::PROPERTY_WRITE | 
    BLECharacteristic::PROPERTY_READ
  );

  //Nastavení zpětného volání pro danou charakteristiku                                                       
  fridgeStateCharacteristic->setCallbacks(new DisplayStateCharacteristicCallback());

  //Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *fridgeInFansCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_IN_FANS_UUID,
    BLECharacteristic::PROPERTY_WRITE | 
    BLECharacteristic::PROPERTY_READ
  );

  //Nastavení zpětného volání pro danou charakteristiku                                                     
  fridgeInFansCharacteristic->setCallbacks(new InFansCharacteristicCallback());

  //Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *powerModeCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_POWER_MODE_UUID,
    BLECharacteristic::PROPERTY_WRITE | 
    BLECharacteristic::PROPERTY_READ
  );

  //Nastavení zpětného volání pro danou charakteristiku                                                         
  powerModeCharacteristic->setCallbacks(new PowerManagerCharacteristicCallback());

  //Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *buzzingOnErrorCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_BUZZING_ON_ERROR_UUID,
    BLECharacteristic::PROPERTY_WRITE | 
    BLECharacteristic::PROPERTY_READ
  );

  //Nastavení zpětného volání pro danou charakteristiku                      
  buzzingOnErrorCharacteristic->setCallbacks(new BuzzingOnErrorCharacteristicCallback());

  //Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *uptimeCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UPTIME_UUID,
    BLECharacteristic::PROPERTY_INDICATE |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_READ
  );

  //Nastavení zpětného volání pro danou charakteristiku                      
  uptimeCharacteristic->setCallbacks(new FridgeUpTimeCharacteristicCallback());

  //Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *factoryCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_FACTORY_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );

  //Nastavení zpětného volání pro danou charakteristiku                                                        
  factoryCharacteristic->setCallbacks(new FactoryResetCharacteristicCallback());

  //Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *doorCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_DOOR_PAUSE_UUID,
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_READ
  );

  //Nastavení zpětného volání pro danou charakteristiku                                                         
  doorCharacteristic->setCallbacks(new DoorCharacteristicCallback());

  //Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *rgbEnableCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_LED_ENABLE_UUID,
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_READ
  );

  //Nastavení zpětného volání pro danou charakteristiku                      
  rgbEnableCharacteristic->setCallbacks(new RGBEnableCharacteristicCallback());

  //Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *rgbColorCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_LED_COLOR_UUID,
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_READ
  );

  //Nastavení zpětného volání pro danou charakteristiku                      
  rgbColorCharacteristic->setCallbacks(new RGBColorCharacteristicCallback());

  //Vytvoření BLE komunikačního kanálu pro komunikaci
  BLECharacteristic *rgbBrightnessCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_LED_BRIGHTNESS_UUID,
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_READ
  );

  //Nastavení zpětného volání pro danou charakteristiku                      
  rgbBrightnessCharacteristic->setCallbacks(new RGBBrightnessCharacteristicCallback());

  //Spuštění begin funkce DataJSONManageru
  DataJSONManager::begin(pService, CHARACTERISTIC_JSON_DATA_UUID, CHARACTERISTIC_READY_TO_SEND_JSON_DATA_UUID, CHARACTERISTIC_JSON_DATA_DELETE_UUID);  

  //Spuštění begin funkce PowerManageru
  PowerManager::begin(pService, CHARACTERISTIC_POWER_MODE_CHANGE_UUID);

  //Spuštění begin funkce ErrorCheckeru
  ErrorChecker::begin(pService, CHARACTERISTIC_ERROR_STATE_UUID, CHARACTERISTIC_FORCE_ERROR_STATE_UUID);

  //Vytvoření nové třídy pro RGB světlo 
  fridge_rgb = new RGBManager(RGB_LED_COUNT, RGB_PIN);
  //Spuštění begin funkce RGBManageru
  fridge_rgb->begin();

  // Zahájení BLE služby
  pService->start();

  //Povolení soukromí na vzdáleném zařízení 
  esp_ble_gap_config_local_privacy(true);

  //Vytvoření zabezpečení
  BLESecurity *pSecurity = new BLESecurity();
  //Nastavení režimu zabezpečení (párování)
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
  //Nastavení schopnosti - žádný vstup, žádný výstup
  pSecurity->setCapability(ESP_IO_CAP_NONE);
  //Inicializace šifrovacího klíče serverem
  pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

  //Uložení advertising do proměnné
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  //Nastavení adversting
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); 
  pAdvertising->setMinPreferred(0x12);
 
  //Zapnutí viditelnosti BLE 
  BLEDevice::startAdvertising();

  //Pokud je zapnutý vývojářký režim, provede se následující 
  if(DEV_MODE) {
    Serial.println("BLE nastaveno, ceka na pripojeni..");
  }
}

void loop() {
  //Uložíme aktuální čas běhu do konstantní proměnné time 
  const unsigned long time = millis();

  //Spuštění loop funkce displeje
  FridgeDisplay::loop();

  //Spuštění loop funkce error kontroly
  ErrorChecker::loop();

  //Spuštění loop funkce piezo manageru 
  PiezoManager::loop();

  //Spuštění loop funkce správce napájení  
  PowerManager::loop();

  //Spuštění loop funkce rgb osvětlení 
  fridge_rgb->loop();

  //Spuštění loop funkce ventilátoru
  cooling_fans_pwm.loop();  

  //Spuštění loop funkce tlačítka
  resetButton->loop();
  //Spuštění funkce pro správu držení tlačítka
  resetButton->button_hold_time(5000, [](){
    //Po podržení tlačítka po dobu 5000ms se provede následující
    //Spuštění funkce pro uvedení zařízení do továního nastavení
    FridgeFactoryReset::factory_reset();
  });

  //Načasování programu
  if(time >= data_send_time_now + data_send_period) {
    data_send_time_now += data_send_period;
    //Spuštění funkce pro aktualizování hodnot o vnitřní teplotě 
    inside_temp_dht->updateTemperature();
    //Spuštění funkce pro aktualizování hodnot o venkovní teplotě 
    outside_temp_dht->updateTemperature();
    //Spuštění funkce pro aktualizování hodnot o teplotě chladiče 
    cooler_temp_ntc->updateTemperature();
    //Pokud je zařízení připojeno k ESP32
    if (devicePaired) {
      //Spuštění funkce pro odeslání vnitřní teploty skrze BLE do připojeného zařízení
      inside_temp_dht->sendTemperature();
      //Spuštění funkce pro odeslání venkovní teploty skrze BLE do připojeného zařízení
      outside_temp_dht->sendTemperature();
      //Spuštění funkce pro odeslání teploty chladiče skrze BLE do připojeného zařízení
      cooler_temp_ntc->sendTemperature();
    }
  }

  //Spuštění loop funkce DataJSONManageru
  DataJSONManager::loop();
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