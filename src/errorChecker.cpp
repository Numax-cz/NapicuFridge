#include <include/errorChecker.h>


void BuzzingOnErrorCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Vypsání hodnoty do konzole
    Serial.println("Odeslání informací o režimu piezo při erroru");
    //Získání dat o režimu piezo z EEPROM 
    uint8_t data = EEPROM.read(PIEZO_ON_ERROR_ADDR);
    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(String(data).c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}

void BuzzingOnErrorCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string msg = pCharacteristic->getValue();

    //Vypsání následujících hodnot do konzole
    Serial.println("Prijata zprava: ");
    Serial.print(msg.c_str());
    Serial.println();

    //Kontrola přijaté zprávy
    //Pokud obsahuje znak 0, při chybě bude piezo vypnuté
    if (msg == "0") {
        //Vypsání hodnoty do konzole
        Serial.println("Vypnutí piezo při chybě");
        //Nastavení statické proměnné na log0
        ErrorChecker::buzzing_on_error = false;
        //Zapsání režimu do EEPROM
        EEPROM.write(PIEZO_ON_ERROR_ADDR, 0);
        //Potvrezní zmeň
        EEPROM.commit();

    }
    //Pokud obsahuje znak 1, při chybě se zapne piezo
    else if (msg == "1") {
        //Vypsání hodnoty do konzole
        Serial.println("Zapnutí piezo při chybě");
        //Nastavení statické proměnné na log1
        ErrorChecker::buzzing_on_error = true;
        //Zapsání režimu do EEPROM
        EEPROM.write(PIEZO_ON_ERROR_ADDR, 1);
        //Potvrezní zmeň
        EEPROM.commit();
    }
}

//Statická funkce pro inicializaci 
void ErrorChecker::begin() {
    //Získání dat o režimu piezo z EEPROM 
    uint8_t data = EEPROM.read(PIEZO_ON_ERROR_ADDR);

    //Pokud není uložená hodnota v EEPROM proveď následující 
    if(data == 0xFF){
        data = DEFAULT_PIEZO_ON_ERROR;
        //Zapsání režimu do EEPROM
        EEPROM.write(PIEZO_ON_ERROR_ADDR, DEFAULT_PIEZO_ON_ERROR);
        //Potvrezní zmeň
        EEPROM.commit();
    }

    //Pokud je data uint8 1 nastaví se true, pokud data uint8 0 nastaví se false
    ErrorChecker::buzzing_on_error = (data) ? true : false;
}

//Loop funkce pro ErrorChecker
void ErrorChecker::loop() {

    //Spuštění funkce pro zkontrolování kritických chyb 
    ErrorChecker::check_fatal_error();
    //Spuštění funkce pro zkontrolování chyb
    ErrorChecker::check_error();
}

//Statická funkce, která zkontroluje chyby
void ErrorChecker::check_error() {
    //Pokud se získaná vnitřní nebo venkovní teplota rovná "nan" provede se následující 
    if(FridgeData.in_temp == "nan" || 
       FridgeData.out_temp == "nan") {
        //Nastavení statické proměnné pro určování zda je lednička v chybě na log1
        ErrorChecker::fridge_error = true;
        //Pokud je zapntué piezo při chybě provede se následující 
        if(ErrorChecker::buzzing_on_error) {
            //Spuštění piezo po dobu 60 sekund
            PiezoManager::time_beep(60);
        } 
        return;
    } else { //Pokud je vše v pořádku provede se následující
        //Pokud je proměnné nastavená na log1 provede se následující
        if(ErrorChecker::fridge_error){
            //Spuštění funkce pro vypnutí peizo
            PiezoManager::stop_beep();
            //Nastavení statické proměnné pro určování zda je lednička v chybě na log1
            ErrorChecker::fridge_error = false;
        } 
    }
}
//Statická funkce, která zkontroluje kritické chyby
void ErrorChecker::check_fatal_error() {

}


