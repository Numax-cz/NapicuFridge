#include <include/fridgeFactoryReset.h>

//Funkce, která uvede zařízení do továrního nastavení 
void FridgeFactoryReset::factory_reset() {
    //Vypsání hodnoty do konzole
    Serial.println("Tovární nastavení...");
    //Následně vymaževe veškeré data z EEPROM (nastavíme veškeré adresy, které využíváme na 0xFF - 255)
    for(int i = 0; i < EEPROM_MAX_SIZE; i++) {
        //Zapsání hodnoty do EEPROM
        EEPROM.write(i, 0xFF);
    }
    //Potvrzení změn
    EEPROM.commit();

    //Spuštění funkce pro pípnutí piezo
    PiezoManager::tone_beep(2, []() {
        //Po ukončení pípání piezo se provede následující
        //Restartování ESP32
        ESP.restart();
    });
}

void FactoryResetCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string prijataZprava = pCharacteristic->getValue();

    //Pokud obsahuje znak 1 provede se následující
    if (prijataZprava == "1") {
        //Spuštění funkce pro uvedení zařízení do továrního nastavení
        FridgeFactoryReset::factory_reset();
    }
}