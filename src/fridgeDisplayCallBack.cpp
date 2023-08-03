#include <include/fridgeDisplay.h>


void DisplayEnableCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Nastavení hodnoty charakteristiky 
    Serial.print("Odeslání informací o stavu displeje");

    int value = FridgeDisplay::get_is_enable() ? 1 : 0;

    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(String(value).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}


void DisplayEnableCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string prijataZprava = pCharacteristic->getValue();

    Serial.print("Prijata zprava: ");
    Serial.print(prijataZprava.c_str());
    Serial.println();


    //Kontrola přijaté zprávy
    //Pokud obsahuje znak 0, vypne se display
    if (prijataZprava == "0") {
        Serial.println("Vypnutí displeje");
        FridgeDisplay::disable_display();
    }
    //Pokud obsahuje znak 1, zapne se display
    else if (prijataZprava == "1") {
        Serial.println("Zapnutí displeje");
        FridgeDisplay::enable_display();
    }
}
