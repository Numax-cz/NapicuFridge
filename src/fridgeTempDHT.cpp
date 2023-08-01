#include <include/fridgeTempDHT.h>

FridgeTempDHT::FridgeTempDHT(int pin, const char* uuid, BLEService* pService, String& value ) : value(value) {
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

//Funkce pro aktualizování aktuálních hodnot
void FridgeTempDHT::updateTemperature() {
    //Získání teploty
    float temp = this->dht->readTemperature();
    //Převedení floatu na string s jedním desetinným místem
    this->value = String(temp, 1);
}

//Funkce, která pošle data skrze BLE do připojeného zařízení
void FridgeTempDHT::sendTemperature() {
    //Nastavení hodnoty charakteristiky 
    this->pCharacteristic->setValue(FridgeData.in_temp.c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    this->pCharacteristic->notify();
    //Vytištění odeslané zprávy po sériové lince
    Serial.print(FridgeData.in_temp);
}
