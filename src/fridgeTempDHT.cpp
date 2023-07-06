#include <include/fridgeTempDHT.h>

FridgeTempDHT::FridgeTempDHT(int pin, const char* uuid, BLEService* pService) {
    this->dht = new DHT(pin, DHT_TYPE);
    // vytvoření BLE komunikačního kanálu pro odesílání (TX)
    this->pCharacteristic = pService->createCharacteristic(
        uuid,
        BLECharacteristic::PROPERTY_INDICATE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_READ
    );
    pCharacteristic->addDescriptor(new BLE2902());
}

FridgeTempDHT::~FridgeTempDHT() {
    delete this->dht;
}

void FridgeTempDHT::begin() {
    this->dht->begin();
}

void FridgeTempDHT::sendTemperature() {
    //Získání teploty
    float temp = this->dht->readTemperature();


    //Převedení floatu na string
    std::ostringstream ss;
    ss << temp;

    this->pCharacteristic->setValue(ss.str());

    //Odeslání zprávy skrze BLE do připojeného zařízení
    this->pCharacteristic->notify();

    //Vytištění odeslané zprávy po sériové lince
    Serial.print(temp);
}