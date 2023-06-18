// Arduino ESP32 a Bluetooth Low Energy
// připojení potřebných knihoven
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <EEPROM.h>
#include <DHT.h>
#include <sstream>
// připojení potřebných lokálních knihoven z /src/include
#include <include/CallBack.h>
#include <include/main.h>
#include <include/fridgeTempDHT.h>

bool zarizeniPripojeno = false;


// inicializace modulu z knihovny

BLEServer* pServer = NULL;

FridgeTempDHT* insideTempDHT = NULL;



void my_gap_event_handler(esp_gap_ble_cb_event_t  event, esp_ble_gap_cb_param_t* param) {
  switch(event) {
      // Proveď následující po ověření zařízení
      case ESP_GAP_BLE_AUTH_CMPL_EVT: {
        if(param->ble_security.auth_cmpl.success) {
          // Vypsání zprávy do konzole
          Serial.print("connected");
          // Získání adresy spárovaného zařízení
          BLEAddress address = BLEAddress(param->ble_security.auth_cmpl.bd_addr);

          // Vymazání white listu
          // esp_ble_gap_clear_whitelist();

          // Přidání adresy do white listu
          BLEDevice::whiteListAdd(address);
        } else {
          // Odpojení zařízení v případě neúspěšného ověření
          esp_ble_gap_disconnect(param->ble_security.auth_cmpl.bd_addr);
        }
        break;
    }   
  }
}


void setup() {
  // zahájení komunikace po sériové lince
  // rychlostí 9600 baud
  Serial.begin(9600);
  // nastavení LED diody jako výstup
  pinMode(CONNECTION_LED, OUTPUT);
  pinMode(TEST_LED, OUTPUT);


  

  // inicializace Bluetooth s nastavením jména zařízení
  BLEDevice::init("NapicuFridge");
  // BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_NO_MITM);

  BLEDevice::setCustomGapHandler(my_gap_event_handler);


  // vytvoření BLE serveru
  pServer = BLEDevice::createServer();
  //pServer->getAdvertising()->setScanFilter(false, true);
  


  pServer->setCallbacks(new ServerCallBack());
  // vytvoření BLE služby
  BLEService *pService = pServer->createService(SERVICE_UUID);



  // vytvoření teploměru pro vnitřní zaznamenávání teploty
  insideTempDHT = new FridgeTempDHT(DHT_INSIDE, CHARACTERISTIC_DHT_INSIDE_TX, pService);
  
  

  
  // vytvoření BLE komunikačního kanálu pro příjem (RX)
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
                                       
  pCharacteristic->setCallbacks(new CharacteristicCallback());
  // zahájení BLE služby
  pService->start();


  // BLESecurity *pSecurity = new BLESecurity();
  // pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
  // pSecurity->setCapability(ESP_IO_CAP_NONE);
  // pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);





  // zapnutí viditelnosti BLE
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); 
  pAdvertising->setMinPreferred(0x12);
 
  BLEDevice::startAdvertising();

  insideTempDHT->begin();

  Serial.println("BLE nastaveno, ceka na pripojeni..");
}
void loop() {
  // pokud je zařízení připojeno k ESP32
  // začneme s odesíláním dat
  if (zarizeniPripojeno == true) {


    insideTempDHT->sendTemperature();

    // vytištění odeslané zprávy po sériové lince
    //Serial.print(temp);
  }
  // pauza před novým během smyčky
  delay(1000);
}