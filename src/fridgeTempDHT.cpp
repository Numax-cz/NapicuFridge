#include <include/fridgeTempDHT.h>

FridgeTempDHT::FridgeTempDHT(int pin, const char* uuid, BLEService* pService, String& value ) : value(value) {
    //Vytvoření nové DHT třídy
    this->dht = new DHT(pin, DHT_TYPE);
    // vytvoření BLE komunikačního kanálu pro odesílání (TX)
    this->pCharacteristic = pService->createCharacteristic(
        uuid,
        BLECharacteristic::PROPERTY_INDICATE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_READ
    );
    //Přiřazení deskriptoru k této charakteristice.
    pCharacteristic->addDescriptor(new BLE2902());

}

FridgeTempDHT::~FridgeTempDHT() {
    //Smazání dht 
    delete this->dht;
}

void FridgeTempDHT::begin() {
    //Spuštění begin funkce v DHT třídy
    this->dht->begin();
}

//Funkce pro aktualizování aktuálních hodnot
void FridgeTempDHT::updateTemperature() {
    //Získání teploty
    float temp = this->dht->readTemperature();
    
    //Kontrola, zda je temp nan
    if(isnan(temp)) {
    
    }

    //Převedení floatu na string s jedním desetinným místem
    this->value = String(temp, 1);
}

//Funkce, která pošle data skrze BLE do připojeného zařízení
void FridgeTempDHT::sendTemperature() {
    //Nastavení hodnoty charakteristiky 
    this->pCharacteristic->setValue(this->value.c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    this->pCharacteristic->notify();
    //Vytištění odeslané zprávy po sériové lince
    // Serial.println(this->value);
}
