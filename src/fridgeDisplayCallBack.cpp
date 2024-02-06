#include <include/fridgeDisplay.h>

void DisplayEnableCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Vypsání hodnoty do konzole
    Serial.println("Odeslání informací o stavu displeje");
    //Získání zda je displej dostupný a převedení na log1/log0
    int value = FridgeDisplay::get_is_enable() ? 1 : 0;
    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(String(value).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}

void DisplayEnableCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string msg = pCharacteristic->getValue();
    //Kontrola přijaté zprávy
    //Pokud obsahuje znak 0, provede se následující 
    if (msg == "0") {
        //Spuštění funkce pro vypnutí displeje
        FridgeDisplay::disable_display();
    }
    //Pokud obsahuje znak 1, provede se následující 
    else if (msg == "1") {
        //Spuštění funkce pro zapnutí displeje
        FridgeDisplay::enable_display();
    }
}

void DisplayStateCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Vypsání hodnoty do konzole
    Serial.print("Odeslání informací o stavu displeje");
    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(String(FridgeDisplay::get_display_state()).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}

void DisplayStateCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string prijataZprava = pCharacteristic->getValue();
    //Proměnná pro ukládání přijaté zprávy v intové formě
    int value_number;
    //Převedení stringu na int a následná kontrola, zda je input správný a je možné převést na číslo
    if (sscanf(prijataZprava.c_str(), "%d", &value_number) == 1){
        //Statické castování intu na fridge_display_state a následené nastavení stavu displeje
        FridgeDisplay::change_display_state(static_cast<fridge_display_state>(value_number));
    }
}