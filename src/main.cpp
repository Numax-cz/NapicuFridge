// Arduino ESP32 a Bluetooth Low Energy
// Připojení potřebných knihoven
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <EEPROM.h>
#include <DHT.h>
#include <sstream>
// Připojení potřebných lokálních knihoven z /src/include
#include <include/CallBack.h>
#include <include/main.h>
#include <include/fridgeTempDHT.h>

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
          Serial.print("connected");
          // Získání adresy spárovaného zařízení
          BLEAddress address = BLEAddress(param->ble_security.auth_cmpl.bd_addr);


          devicePaired = true;
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
  // Zahájení komunikace po sériové lince
  // Rychlostí 9600 baud
  Serial.begin(9600);
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
  //pServer->getAdvertising()->setScanFilter(false, true);
  


  pServer->setCallbacks(new ServerCallBack());
  // Vytvoření BLE služby
  BLEService *pService = pServer->createService(SERVICE_UUID);



  // Vytvoření teploměru pro vnitřní zaznamenávání teploty
  insideTempDHT = new FridgeTempDHT(DHT_INSIDE, CHARACTERISTIC_DHT_INSIDE_TX, pService);
  
  

  
  // Vytvoření BLE komunikačního kanálu pro příjem (RX)
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
                                       
  pCharacteristic->setCallbacks(new CharacteristicCallback());
  // Zahájení BLE služby
  pService->start();


  // BLESecurity *pSecurity = new BLESecurity();
  // pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
  // pSecurity->setCapability(ESP_IO_CAP_NONE);
  // pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);





  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); 
  pAdvertising->setMinPreferred(0x12);
 
  // Zapnutí viditelnosti BLE 
  BLEDevice::startAdvertising();

  insideTempDHT->begin();

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