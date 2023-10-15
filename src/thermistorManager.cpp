#include <include/thermistorManager.h>

/**
 * @brief Konstruktor pro vytvoření nové třídy
 *
 * @param pin Pin pro získávání dat z termistoru
 * @param uuid UUID pro odesílání dat
 * @param pService BLE služba
 * @param value Reference hodnoty na kterou se budou ukládat získané data
 * @param referenceResistance Referenční hodnota rezistoru 
 * @param nominalResistance  Nominální odpor
 * @param nominalTemperatureCelsius Nominální teplota 
 * @param bValue Beta hodnota
 * @param adcResolution Analogové rozlišení
 */
ThermistorManager::ThermistorManager(int pin, const char* uuid, BLEService* pService, String& value, 
    double referenceResistance, double nominalResistance, double nominalTemperatureCelsius, double bValue, int adcResolution): value(value) {
    //Vytvoření nové ntc thermistor třídy
    this->ntc_thermistor = new NTC_Thermistor(pin, referenceResistance, nominalResistance, nominalTemperatureCelsius, bValue, adcResolution);
    //Vytvoření BLE komunikačního kanálu pro odesílání (TX)
    this->pCharacteristic = pService->createCharacteristic(
        uuid,
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_READ
    );
    //Přiřazení deskriptoru k této charakteristice.
    pCharacteristic->addDescriptor(new BLE2902());

}

//Dekonstruktor pro třídu 
ThermistorManager::~ThermistorManager() {
    //Smazání thermistor třídy
    delete this->ntc_thermistor;
}

//Funkce pro aktualizování aktuálních hodnot
void ThermistorManager::updateTemperature() {
    //Získání teploty z ntc termistoru v celsius
    double temp = this->ntc_thermistor->readCelsius();
    //Pokud se proměnná celsius rovná -273.15 (nesmyslná hodnota při nezískání dat např. vadný senzor) provede zapsání NAN
    if(temp == -273.15) temp = NAN;
    //Převedení floatu na string s jedním desetinným místem
    this->value = String(temp, 1);
}

//Funkce, která pošle data skrze BLE do připojeného zařízení
void ThermistorManager::sendTemperature() {
    //Nastavení hodnoty charakteristiky 
    this->pCharacteristic->setValue(this->value.c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    this->pCharacteristic->notify();
    //Vytištění odeslané zprávy po sériové lince
    Serial.println(this->value);
}