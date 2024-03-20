#include <include/powerManager.h>

void PowerManagerCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Vypsání hodnoty do konzole
    Serial.println("Odeslání informací o režimu napájení");
    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(String(PowerManager::get_selected_mode()).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}

void PowerManagerCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string msg = pCharacteristic->getValue();
    //Zkouška části kódu
    try {
        //Převod řetězce na celé číslo
        int number = std::stoi(msg); 
        //Spuštění funkce pro změnu režimu
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
    //Uložení log1/log0, podle toho zda je relátko otevřené/zavřené
    int value = relay_in_fans->get_is_open() ? 1 : 0;
    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(String(value).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}

void InFansCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string msg = pCharacteristic->getValue();
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

void DoorCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string msg = pCharacteristic->getValue();
    //Kontrola přijaté zprávy
    //Pokud obsahuje znak 0, lednička se nepozastaví při otevřených dveří
    if (msg == "0") {
        //Nastavení proměnné určující, zda se má lednička pozastavit při otevřených dveří na log0
        PowerManager::fridge_pause_on_door_open = 0;
        //Zapsání dat do EEPROM
        EEPROM.write(FRIDGE_PAUSE_ON_DOOR_OPEN_ADDR, PowerManager::fridge_pause_on_door_open);
        //Potvrezní zmeň
        EEPROM.commit();
    }
    //Pokud obsahuje znak 1, lednička se pozastaví při otevřených dveří
    else if (msg == "1") {
        //Nastavení proměnné určující, zda se má lednička pozastavit při otevřených dveří na log1
        PowerManager::fridge_pause_on_door_open = 1;
        //Zapsání dat do EEPROM
        EEPROM.write(FRIDGE_PAUSE_ON_DOOR_OPEN_ADDR, PowerManager::fridge_pause_on_door_open);
        //Potvrezní zmeň
        EEPROM.commit();
    }
}

void DoorCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Vypsání hodnoty do konzole
    Serial.print("Odeslání informací o stavu nastavení dveří");
    //Získání dat z EEPROM 
    uint8_t data = EEPROM.read(FRIDGE_PAUSE_ON_DOOR_OPEN_ADDR);
    //Pokud není uložena hodnota v EEPROM provede se následující 
    if(data == 0xFF) {
        //Nastaví se výchozí hodnota podle definice
        data = DEFAULT_FRIDGE_PAUSE_ON_DOOR_OPEN;
    }
    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(String(data).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}

//Begin funkce pro PowerManager
void PowerManager::begin(BLEService* pService, const char* notify_uuid) {
    //Nastavení digitálního potenciometru
    digital_potentiometer->set(100);
    // vytvoření BLE komunikačního kanálu pro odesílání (TX)
    PowerManager::pCharacteristic = pService->createCharacteristic(
        notify_uuid,
        BLECharacteristic::PROPERTY_INDICATE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_READ
    );
    //Přiřazení deskriptoru k této charakteristice.
    PowerManager::pCharacteristic->addDescriptor(new BLE2902());

    //Spuštění begin funkce pro inicializaci chladících ventilátorů 
    cooling_fans_pwm.begin();
    //Spuštění funkce pro načtení nastavení 
    PowerManager::load_config_from_eeprom();
}

//Statická funkce, která pošle připojenému zařízení aktuální režim napájení + nastavení vnitřních ventilátorů
void PowerManager::notify_power_config() {
    //Vypsání hodnoty do konzole
    Serial.println("Odeslání informací o režimu napájení");
    //Vypsání hodnoty do konzole
    Serial.println("Odeslání informací o stavu relátka vnitřních ventilátorů");
    //Získání dat o stavu relátek pro vnitřní ventilátory
    int relay_in_fans_value = relay_in_fans->get_is_open() ? 1 : 0;
    //Nastavení hodnoty charakteristiky 
    PowerManager::pCharacteristic->setValue((String(PowerManager::selected_mode) + String(relay_in_fans_value)).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    PowerManager::pCharacteristic->notify();
}

//Funkce, která načte veškerá nastavení z EEPROM
void PowerManager::load_config_from_eeprom() {
    //Proměnná pro uložení dat z EEPROM
    uint8_t data = EEPROM.read(POWER_MODE_EEPROM_ADDR);
    //Pokud není uložená hodnota v EEPROM proveď následující 
    if(data == 0xFF) {
        //Nastavení výchozí hodnoty
        data = DEFAULT_POWER_MODE;
    }
    //Spuštění funkce pro nastavení režimu výkonu
    PowerManager::change_power_mode(data);
    //Pokud lednička není nastavená na režim vypnuto provede se následující
    if(data != FRIDGE_OFF_POWER) {
        //Spuštění funkce pro inicializaci vnitřních ventilátorů
        PowerManager::begin_in_fans();
    }    
    //Získání dat z EEPROM 
    uint8_t door_data = EEPROM.read(FRIDGE_PAUSE_ON_DOOR_OPEN_ADDR);
    //Pokud je uložena hodnota v EEPROM provede se následující 
    if(door_data != 0xFF) {
        //Nazstavení hodnoty z EEPROM
        PowerManager::fridge_pause_on_door_open = door_data;
    } else {
        //Nastavení výchozí hodnoty
        PowerManager::fridge_pause_on_door_open = DEFAULT_FRIDGE_PAUSE_ON_DOOR_OPEN;
    }
    //Získání dat z EEPROM 
    uint8_t led_data = EEPROM.read(LED_AVAILABLE_EEPROM_ADDR);
    //Pokud je uložena hodnota v EEPROM provede se následující 
    if(led_data != 0xFF) {
        //Nazstavení hodnoty z EEPROM
        PowerManager::fridge_led_enable_on_door_open = led_data;
    } else {
        //Nastavení výchozí hodnoty
        PowerManager::fridge_led_enable_on_door_open = DEFAULT_LED_ENABLE;
    }
}

/**
 * @brief Statická funkce pro změnu napájecího režimu
 * 
 * @param mode Režim napájení který se nastaví
 */
void PowerManager::change_power_mode(int mode) {
    //Pokud se lednička nachází v kritické chybě provede se následující 
    if(ErrorChecker::is_fridge_on_fatal_error()) return;
    //Zkontroluje, zda převedené číslo odpovídá některé hodnotě enumerace
    if (mode == FRIDGE_OFF_POWER) {
        //Spuštění funkce pro smazání souboru ukládající naměřené hodnoty
        DataJSONManager::delete_file();
        //Spuštění funkce pro vypnutí chlazení 
        PowerManager::power_off();
    } 
    else if (mode == FRIDGE_PAUSED) {
        PowerManager::power_off();
    }
    else if (mode == FRIDGE_MAX_POWER) {
        //Nastavení digitálního potenciometru
        digital_potentiometer->set(100);
        //Zavolání funkce pro zavření relátka ovládající napájecí režim
        relay_peltier_power_mode->close();
        //Zavolání funkce pro zapnutí chladícího systému
        PowerManager::power_on();
        //Nastavení otáček ventilátorů
        cooling_fans_pwm.set_fan_speed_in_percentage(100);
    } else if (mode == FRIDGE_NORMAL_POWER) {
        //Nastavení digitálního potenciometru
        digital_potentiometer->set(100);
        //Zavolání funkce pro otevření relátka ovládající napájecí režim
        relay_peltier_power_mode->open();
        //Zavolání funkce pro zapnutí chladícího systému
        PowerManager::power_on();
        //Nastavení otáček ventilátorů
        cooling_fans_pwm.set_fan_speed_in_percentage(60);
    } else if (mode == FRIDGE_ECO_POWER) {
        //Nastavení digitálního potenciometru
        digital_potentiometer->set(80);
        //Zavolání funkce pro otevření relátka ovládající napájecí režim
        relay_peltier_power_mode->open();
        //Zavolání funkce pro zapnutí chladícího systému
        PowerManager::power_on();
        //Nastavení otáček ventilátorů
        cooling_fans_pwm.set_fan_speed_in_percentage(40);
    }

    //Uložení nastaveného módu do proměnné
    PowerManager::selected_mode = mode;
    //Pokud se nastavený mód nerovná pauze provede se následující 
    if(mode != FRIDGE_PAUSED) {
        //Zapsání režimu do EEPROM
        EEPROM.write(POWER_MODE_EEPROM_ADDR, mode);
        //Potvrezní zmeň
        EEPROM.commit();
    }
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
    //Pokud jsou dveře otevřeny provede se následující 
    if(digitalRead(DOOR_PIN) == LOW) {
        //Pokud je proměnná určující, zda jsou dveře otevřeny nastavena na log1
        if(!PowerManager::is_door_open) {
            //Pokud je povolena pauza chladícího systému při otevření provede se následující 
            if(PowerManager::fridge_pause_on_door_open) {
                //Spuštění funkce pro uvedení ledničky do pauzy 
                PowerManager::pause_fridge();
                //Nastavení proměnné, určující, zda jsou dveře otevřeny na log1
                PowerManager::is_door_open = true;
            }
            //Pokud je proměnná určující, zda se má LED osvětlení zapnout při otevření dveří nastavena na log1
            if(PowerManager::fridge_led_enable_on_door_open) {
                //Spuštění funkce pro zapnutí RGB světla
                fridge_rgb->turn_on();
            }
        }
    } else { //Pokud jsou dveře zavřené provede se následující 
        //Pokud je proměnná určující, zda jsou dveře otevřeny nastavena na log1
        if(PowerManager::is_door_open) {
            //Spuštění funkce, která zruší pauzu ledničky
            PowerManager::cancel_pause_fridge();
            //Nastavení proměnné, určující, zda jsou dveře otevřeny na log0
            PowerManager::is_door_open = false;
            //Spuštění funkce pro vypnutí RGB světla
            fridge_rgb->turn_off();
        }
    }
}
//Funkce, která uvede ledničku do pauzy
void PowerManager::pause_fridge() {
    //Spuštění funkce pro přepnutí napájecího režimu na stav "vypnuto"
    PowerManager::change_power_mode(FRIDGE_PAUSED);
    //Spuštění funkce pro oznámení o změně chladícího systému připojenému zařízení 
    PowerManager::notify_power_config();
}

//Funkce, která zruší pauzu ledničky
void PowerManager::cancel_pause_fridge() {
    //Spuštění funkce proč načtení veškerých nastavení z EEPROM
    PowerManager::load_config_from_eeprom();
    //Spuštění funkce pro oznámení o změně chladícího systému připojenému zařízení 
    PowerManager::notify_power_config();
}

//Funkce pro vypnutí celého chladícího systému
void PowerManager::power_off() {
    //Zavření relátka pro režim napájení
    relay_peltier_power_mode->close();
    //Zavření relátka pro hlavní napájení peltierů
    relay_peltier->close();
    //Spuštění funkce pro zavření relátka vnitřních ventilátorů 
    relay_in_fans->close();
    //Spuštění funkce pro vypnutí chladících ventilátorů pokud ventilátory běží
    if (cooling_fans_pwm.get_is_fan_running())  PowerManager::turn_off_cooling_fans();
}

//Funkce pro zapnutí celého chladícího systému
void PowerManager::power_on() {
    relay_peltier->open();
    relay_cooling_fans->open();
}

//Funkce, která vrací zda je chladící systém zapnutý 
bool PowerManager::is_power_on() {
    //Proměnná pro uložení dat z EEPROM
    uint8_t data = EEPROM.read(POWER_MODE_EEPROM_ADDR);
    return (data != FRIDGE_OFF_POWER);
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

//Funkce, která vrátí vybraní napájecí režim
int PowerManager::get_selected_mode() {
    return PowerManager::selected_mode;
}