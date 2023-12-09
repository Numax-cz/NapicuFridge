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


void RGBColorCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Vypsání hodnoty do konzole
    Serial.println("Odeslání informací o barvy LED osvětlení");
    //Získání hodnoty červené z EEPROM 
    uint8_t R = EEPROM.read(LED_COLOR_EEPROM_ADDR);
    //Získání hodnoty zelené z EEPROM 
    uint8_t G = EEPROM.read(LED_COLOR_EEPROM_ADDR + 1);
    //Získání hodnoty modré z EEPROM 
    uint8_t B = EEPROM.read(LED_COLOR_EEPROM_ADDR + 2);
    //Sjednocení veškerých hodnot
    String color = String(R) + "," + String(G) + "," + String(B);
    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(color.c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}

void RGBColorCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string msg = pCharacteristic->getValue();
    //Proměnné pro uložení hodnoty červené, zelené, modré
    uint8_t r, g, b;
    
    std::istringstream iss(msg);
    // Vytvoření vektoru pro uchování hodnot RGB
    std::vector<int> rgbValues;

    // Rozdělení řetězce podle čárky a přidání hodnot do vektoru
    std::string token;
    while (std::getline(iss, token, ',')) {
        // Převod řetězce na číslo a přidání do vektoru
        rgbValues.push_back(std::stoi(token));
    }

    //Nastavení hodnoty červené barvy
    r = rgbValues[0];
    //Nastavení hodnoty zelené barvy
    g = rgbValues[1];
    //Nastavení hodnoty modré barvy
    b = rgbValues[2];

    //Uložení hodnoty červené barvy do EEPROM
    EEPROM.write(LED_COLOR_EEPROM_ADDR, r);
    //Uložení hodnoty zelené barvy do EEPROM
    EEPROM.write(LED_COLOR_EEPROM_ADDR + 1, g);
    //Uložení hodnoty modré barvy do EEPROM
    EEPROM.write(LED_COLOR_EEPROM_ADDR + 2, b);

    //Nastavení barvy led osvětlení
    fridge_rgb->setColor(r, g, b);
}