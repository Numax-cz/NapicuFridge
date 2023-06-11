// Arduino ESP32 a Bluetooth Low Energy
// připojení potřebných knihoven
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <EEPROM.h>

// připojení potřebných lokálních knihoven z /src/include
#include <include/CallBack.h>
#include <include/main.h>


bool zarizeniPripojeno = false;
std::string prijataZprava;

// inicializace modulu z knihovny
BLECharacteristic *pCharacteristic;


BLEServer* pServer = NULL;




void my_gap_event_handler(esp_gap_ble_cb_event_t  event, esp_ble_gap_cb_param_t* param) {
  switch(event){
      case ESP_GAP_BLE_AUTH_CMPL_EVT:{
        Serial.print("connected");
        BLEAddress address = BLEAddress(param->ble_security.auth_cmpl.bd_addr);
        BLEDevice::whiteListAdd(address);
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
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);

  BLEDevice::setCustomGapHandler(my_gap_event_handler);
    

  // vytvoření BLE serveru
  pServer = BLEDevice::createServer();
  //pServer->getAdvertising()->setScanFilter(false, true);




  pServer->setCallbacks(new ServerCallBack());
  // vytvoření BLE služby
  BLEService *pService = pServer->createService(SERVICE_UUID);



  // vytvoření BLE komunikačního kanálu pro odesílání (TX)
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());
  // vytvoření BLE komunikačního kanálu pro příjem (RX)
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
                                       
  pCharacteristic->setCallbacks(new CharacteristicCallback());
  // zahájení BLE služby
  pService->start();


  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
  pSecurity->setCapability(ESP_IO_CAP_NONE);
  pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);




  // zapnutí viditelnosti BLE
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); 
  pAdvertising->setMinPreferred(0x12);
 
  BLEDevice::startAdvertising();


  Serial.println("BLE nastaveno, ceka na pripojeni..");
}
void loop() {
  // pokud je zařízení připojeno k ESP32
  // začneme s odesíláním dat
  if (zarizeniPripojeno == true) {
    // načtení dat z analogového pinu s dělením pro
    // vytvoření čísla s desetinnou hodnotou
    float data = analogRead(readPin) / 3.456;
    // vytvoření zprávy z textu a naměřených dat
    // znak \n slouží k odřádkování
    String zprava = "Analog: ";
    zprava += data;
    zprava += "\n";


    // vytvoření dočasné proměnné, do které
    // je převedna zpráva na znaky
    char zpravaChar[zprava.length() + 1];
    zprava.toCharArray(zpravaChar, zprava.length() + 1);
    // přepsání zprávy do BLE služby
    pCharacteristic->setValue(zpravaChar);
    // odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
    // vytištění odeslané zprávy po sériové lince
    Serial.print("*** Odeslana zprava: ");
    Serial.print(zprava);
  }
  // pauza před novým během smyčky
  delay(1000);
}