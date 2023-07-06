// Arduino ESP32 a Bluetooth Low Energy

// Připojení potřebných lokálních knihoven z /src/include
#include <include/main.h>

bool devicePaired = false;


// Inicializace modulu z knihovny

BLEServer* pServer = NULL;

FridgeTempDHT* insideTempDHT = NULL;



void my_gap_event_handler(esp_gap_ble_cb_event_t  event, esp_ble_gap_cb_param_t* param) {
  switch(event) {
      // Proveď následující po ověření zařízení
      case ESP_GAP_BLE_AUTH_CMPL_EVT: {
        if(param->ble_security.auth_cmpl.success) {
          // Vypsání zprávy do konzole
          Serial.println("connected");
          // Získání adresy spárovaného zařízení
          BLEAddress address = BLEAddress(param->ble_security.auth_cmpl.bd_addr);
          
          // Serial.printf(address.toString().c_str());

      
          //Získání reprezentační MAC adresy.
          esp_bd_addr_t* address_native = address.getNative();

          for (int i = 0; i < EEPROM_SIZE; ++i) {
            //Zapsání hodnoty do EEPROM
            EEPROM.write(MAC_ADDRESS_EEPROM_ADDR + i, (*address_native)[i]);
          }

          //Potvrzení změn
          EEPROM.commit();


          devicePaired = true;


          // // Přidání adresy do white listu
          // BLEDevice::whiteListAdd(address);
          // //Zapnutí white listu
          // set_whitelist();

          //BLEDevice::startAdvertising();
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

  //Inicializace paměti EEPROM
  if (!EEPROM.begin(6)) {
    //Vypsání hodnoty do konzole
    Serial.println("EEPROM inicializace byla neúspěšná");
    while (true);
  } else {
    //Vypsání hodnoty do konzole
    Serial.println("EEPROM je dostupné");
  }


  // Nastavení LED diody jako výstup
  pinMode(CONNECTION_LED, OUTPUT);
  pinMode(TEST_LED, OUTPUT);



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
  insideTempDHT = new FridgeTempDHT(DHT_INSIDE, CHARACTERISTIC_DHT_INSIDE_TX, pService);
  
  insideTempDHT->begin();
  

  
  // Vytvoření BLE komunikačního kanálu pro příjem (RX)
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
                                       
  pCharacteristic->setCallbacks(new CharacteristicCallback());
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
  // Pokud je zařízení připojeno k ESP32
  // Začneme s odesíláním dat
  if (devicePaired == true) {


    
    insideTempDHT->sendTemperature();

    // vytištění odeslané zprávy po sériové lince
    //Serial.print(temp);
  }
  // Pauza před novým během smyčky
  delay(1000);
}


