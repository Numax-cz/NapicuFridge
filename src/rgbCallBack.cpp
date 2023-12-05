/**
 * @file rgbCallBack.cpp
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-4-12
 * 
 * @copyright Copyright (c) 2023
*/

//Připojení hlavní knihovny 
#include <include/rgbManager.h>

void RGBEnableCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Vypsání hodnoty do konzole
    Serial.println("Odeslání informací o dostupnosti LED osvětlení");
    //Získání dat o dostupnosti LED světla z EEPROM 
    uint8_t data = EEPROM.read(LED_AVAILABLE_EEPROM_ADDR);
    //Pokud není uložená hodnota v EEPROM provede se následující (rovná se 255)
    if(data == 0xFF) {
        //Nastavení výchozí hodnoty
        data = DEFAULT_LED_ENABLE;
    } 
    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(String(data).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}

void RGBEnableCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string msg = pCharacteristic->getValue();
    //Pokud se přijatá zprává rovná "0" provede se následující 
    if(msg == "0") {
        //Zapsání log0 hodnoty do EEPROM
        EEPROM.write(LED_AVAILABLE_EEPROM_ADDR, 0);
        //Nastavení proměnné určující, zda se má LED osvětlení zapnout při otevření dveří na log0
        PowerManager::fridge_led_enable_on_door_open = 0;
        //Spuštění funkce pro vypnutí RGB světla
        fridge_rgb->turn_off();
        //Potvrzení změn
        EEPROM.commit();
    } 
    //Pokud se přijatá zprává rovná "1" provede se následující
    else if (msg == "1") {  
        //Zapsání log1 hodnoty do EEPROM
        EEPROM.write(LED_AVAILABLE_EEPROM_ADDR, 1);
        //Nastavení proměnné určující, zda se má LED osvětlení zapnout při otevření dveří na log1
        PowerManager::fridge_led_enable_on_door_open = 1;
        //Potvrzení změn
        EEPROM.commit();
    }
}
