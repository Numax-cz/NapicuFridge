#include <include/powerManager.h>





void PowerManagerCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Vypsání hodnoty do konzole
    Serial.println("Odeslání informací o režimu napájení");
    //Získání dat o režimu napájení z EEPROM 
    uint8_t data = EEPROM.read(POWER_MODE_EEPROM_ADDR);
    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(String(data).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}

void PowerManagerCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string msg = pCharacteristic->getValue();

    //Vypsání následujících hodnot do konzole
    Serial.print("Prijata zprava: ");
    Serial.print(msg.c_str());
    Serial.println();


    //Zkouška části kódu
    try {
        //Převod řetězce na celé číslo
        int number = std::stoi(msg); 

        PowerManager::change_power_mode(number);


    } catch (std::invalid_argument const &e) {
        //Vypsání chybné hlášky do konzole
        Serial.print("Nelze převést řetězec na číslo: ");
        //Vypsání chyby do konzole
        Serial.print(e.what());
    } catch (std::out_of_range const &e) {
        //Vypsání chybné hlášky do konzole
        Serial.print("Přetečení při převodu řetězce na číslo: ");
        //Vypsání chyby do konzole
        Serial.print(e.what());
    }
}



void InFansCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Vypsání hodnoty do konzole
    Serial.println("Odeslání informací o stavu relátka vnitřních ventilátorů");

    int value = relay_in_fans->get_is_open() ? 1 : 0;

    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(String(value).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}

void InFansCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string msg = pCharacteristic->getValue();

    //Vypsání následujících hodnot do konzole
    Serial.println("Prijata zprava: ");
    Serial.print(msg.c_str());
    Serial.println();

    //Kontrola přijaté zprávy
    //Pokud obsahuje znak 0, vypnou se vnitřní ventilátory
    if (msg == "0") {
        //Vypsání hodnoty do konzole
        Serial.println("Vypnutí vnitřních ventilátorů");
        //Zavolání funkce pro vypnutí displeje
        PowerManager::turn_off_in_fans();
    }
    //Pokud obsahuje znak 1, zapnou se vnitřní ventilátory
    else if (msg == "1") {
        //Vypsání hodnoty do konzole
        Serial.println("Zapnutí vnitřních ventilátorů");
        //Zavolání funkce pro zapnutí displeje
        PowerManager::turn_on_in_fans();
    }
}




//Begin funkce pro PowerManager
void PowerManager::begin() {
    //Proměnná pro uložení dat z EEPROM
    uint8_t data = EEPROM.read(POWER_MODE_EEPROM_ADDR);
    //Spuštění begin funkce pro inicializaci chladících ventilátorů 
    cooling_fans_pwm.begin();

    //Pokud není uložená hodnota v EEPROM proveď následující 
    if(data == 0xFF) {
        data = DEFAULT_POWER_MODE;
        //TODO IDK jestli to tu bude 
        EEPROM.write(POWER_MODE_EEPROM_ADDR, data);
        EEPROM.commit();
    }

    //Spuštění funkce pro nastavení režimu výkonu
    PowerManager::change_power_mode(data);

    //Pokud lednička není nastavená na režim vypnuto provede se následující
    if(data != FRIDGE_OFF_POWER) {
        //Spuštění funkce pro inicializaci vnitřních ventilátorů
        PowerManager::begin_in_fans();
    }    
}

//Statická funkce pro změnu napájecího režimu
void PowerManager::change_power_mode(int mode) {
    
    //Zkontroluje, zda převedené číslo odpovídá některé hodnotě enumerace
    if (mode == FRIDGE_OFF_POWER) {
        PowerManager::power_off();
    } else if (mode == FRIDGE_MAX_POWER) {
        relay_peltier_power_mode->close();
        //Zavolání funkce pro zapnutí chladícího systému
        PowerManager::power_on();
    } else if (mode == FRIDGE_NORMAL_POWER) {
        relay_peltier_power_mode->open();
        //Zavolání funkce pro zapnutí chladícího systému
        PowerManager::power_on();
    } else if (mode == FRIDGE_ECO_POWER) {
        relay_peltier_power_mode->open();
        //Zavolání funkce pro zapnutí chladícího systému
        PowerManager::power_on();
    } else {

    }

    //TODO IF 
    //Zapsání režimu do EEPROM
    EEPROM.write(POWER_MODE_EEPROM_ADDR, mode);
    //Potvrezní zmeň
    EEPROM.commit();
}

//Funkce pro inicializaci vnitřních ventilátorů
void PowerManager::begin_in_fans() {
    //Proměnná pro uložení dat z EEPROM
    uint8_t data = EEPROM.read(IN_FANS_EEPROM_ADDR);

    //Pokud není uložená hodnota v EEPROM proveď následující 
    if(data == 0xFF) {
        //Nastaví se výchozí hodnota
        data = IN_FANS_DEFAULT_AVAILABLE;
    } 
    //Pokud je data rovno log1
    if(data == 1) {
        //Spuštění funkce pro zapnutí vnitřních ventilátorů
        PowerManager::turn_on_in_fans();
    } else {
        //Spuštění funkce pro vypnutí vnitřních ventilátorů
        PowerManager::turn_off_in_fans();
    }
}

//Statická loop funkce pro PowerManager
void PowerManager::loop() {

}

//Funkce pro vypnutí celého chladícího systému
void PowerManager::power_off() {
    //Zavření relátka pro režim napájení
    relay_peltier_power_mode->close();
    //Zavření relátka pro hlavní napájení peltierů
    relay_peltier->close();
    //Spuštění funkce pro vypnutí vnitřních ventilátorů
    PowerManager::turn_off_in_fans();
    //Spuštění funkce pro vypnutí chladících ventilátorů
    PowerManager::turn_off_cooling_fans();
    // //Zapsání hodnoty do EEPROM
    // EEPROM.write(POWER_MODE_EEPROM_ADDR, FRIDGE_OFF_POWER);
    // //Potvrezní zmeň
    // EEPROM.commit();
}

//Funkce pro zapnutí celého chladícího systému
void PowerManager::power_on() {
    relay_peltier->open();
    relay_cooling_fans->open();
}

//Funkce pro vypnutí chladících ventilátorů
void PowerManager::turn_off_cooling_fans() {
    //Spuštění funkce pro zavření relátka chladících ventilátorů (PWM)
    relay_cooling_fans->close();
}

//Funkce pro zapnutí chladících ventilátorů
void PowerManager::turn_on_cooling_fans() {
    //Spuštění funkce pro otevření relátka chladících ventilátorů (PWM)
    relay_cooling_fans->open();
}

//Funkce pro vypnutí vnitřních ventilátorů
void PowerManager::turn_off_in_fans() {
    //Zapsání log0 hodnoty do EEPROM
    EEPROM.write(IN_FANS_EEPROM_ADDR, 0);
    //Spuštění funkce pro zavření relátka vnitřních ventilátorů
    relay_in_fans->close();
    //Potvrzení změn
    EEPROM.commit();
}

//Funkce pro zapnutí vnitřních ventilátorů
void PowerManager::turn_on_in_fans() {
    //Zapsání log1 hodnoty do EEPROM
    EEPROM.write(IN_FANS_EEPROM_ADDR, 1);
    //Spuštění funkce pro otevření relátka vnitřních ventilátorů
    relay_in_fans->open();
    //Potvrzení změn
    EEPROM.commit();
}

