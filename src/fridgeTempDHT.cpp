#include <include/fridgeTempDHT.h>

/**
 * @brief Konstruktor třídy
 * 
 * @param pin Datový pin DHT senzoru 
 * @param uuid UUID pro komunikaci 
 * @param pService BLE služba
 * @param value Reference hodnoty pro teplotu
 */
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

//Dekonstruktoru
FridgeTempDHT::~FridgeTempDHT() {
    //Smazání dht 
    delete this->dht;
}

//Funkce pro inicializaci DHT senzoru 
void FridgeTempDHT::begin() {
    //Spuštění begin funkce v DHT třídy
    this->dht->begin();
}

//Funkce pro aktualizování teploty
void FridgeTempDHT::updateTemperature() {
    //Získání teploty
    float temp = this->dht->readTemperature();
    //Převedení floatu na string s jedním desetinným místem
    this->value = String(temp, 1);
}

//Funkce, která pošle data skrze BLE do připojeného zařízení
void FridgeTempDHT::sendTemperature() {
    //Nastavení hodnoty charakteristiky 
    this->pCharacteristic->setValue(this->value.c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    this->pCharacteristic->notify();
}
