// Arduino ESP32 a Bluetooth Low Energy

// připojení potřebných knihoven
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>



// definice propojovacích pinů
// pro analogový vstup a LED diodu
#define readPin 32
#define LED 19
// inicializace modulu z knihovny
BLECharacteristic *pCharacteristic;
// proměnná pro kontrolu připojených zařízení
bool zarizeniPripojeno = false;
// proměnná pro ukládání přijaté zprávy
std::string prijataZprava;
// definice unikátních ID pro různé služby,
// pro vlastní UUID využijte generátor
// https://www.uuidgenerator.net/
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// třída pro kontrolu připojení
class MyServerCallbacks: public BLEServerCallbacks {
    // při spojení zařízení nastav proměnnou na log1
    void onConnect(BLEServer* pServer) {
      zarizeniPripojeno = true;
    };
    // při odpojení zařízení nastav proměnnou na log0
    void onDisconnect(BLEServer* pServer) {
      zarizeniPripojeno = false;
    }
};

// třída pro příjem zprávy
class MyCallbacks: public BLECharacteristicCallbacks {
    // při příjmu zprávy proveď následující
    void onWrite(BLECharacteristic *pCharacteristic) {
      
      // načti přijatou zprávu do proměnné
      prijataZprava = pCharacteristic->getValue();
      // pokud není zpráva prázdná, vypiš její obsah
      // po znacích po sériové lince
      if (prijataZprava.length() > 0) {
        Serial.print("Prijata zprava: ");
        for (int i = 0; i < prijataZprava.length(); i++) {
          Serial.print(prijataZprava[i]);
        }
        Serial.println();
        // kontrola přijaté zprávy
        // pokud obsahuje znak A, rozsviť LED diodu
        if (prijataZprava.find("A") != -1) {
          Serial.println("Zapnutí LED!");
          digitalWrite(LED, HIGH);
        }
        // pokud obsahuje znak B, zhasni LED diodu
        else if (prijataZprava.find("B") != -1) {
          Serial.println("Vypnutí LED!");
          digitalWrite(LED, LOW);
        }
      }
    }
};
void setup() {
  // zahájení komunikace po sériové lince
  // rychlostí 9600 baud
  Serial.begin(9600);
  // nastavení LED diody jako výstup
  pinMode(LED, OUTPUT);
  // inicializace Bluetooth s nastavením jména zařízení
  BLEDevice::init("NapicuFridge");
  // vytvoření BLE serveru
  BLEServer *pServer = BLEDevice::createServer();

  pServer->setCallbacks(new MyServerCallbacks());
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
                                       
  pCharacteristic->setCallbacks(new MyCallbacks());
  // zahájení BLE služby
  pService->start();
  
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