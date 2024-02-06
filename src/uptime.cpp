#include <include/uptime.h>

void FridgeUpTimeCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Vypsání hodnoty do konzole
    Serial.print("Odeslání informaci o uptime");
    //Uložíme aktuální čas běhu do konstantní proměnné time 
    const unsigned long time = millis();
    //Spuštění funkce pro nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(String(time).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}