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

//Třída pro získávání informací od chybách 
void ErrorStateCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    //Vypsání hodnoty do konzole
    Serial.println("Odeslání informací o chybách");
    //Nastavení hodnoty charakteristiky 
    pCharacteristic->setValue(ErrorChecker::get_error_log().c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    pCharacteristic->notify();
}

//Funkce pro inicializaci 
void ErrorChecker::begin(BLEService* pService, const char* notify_uuid, const char* force_uuid) {
    //Vytvoření BLE komunikačního kanálu pro odesílání (TX)
    ErrorChecker::pCharacteristicNotify = pService->createCharacteristic(
        notify_uuid,
        BLECharacteristic::PROPERTY_INDICATE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_READ
    );
    //Přiřazení deskriptoru k této charakteristice.
    ErrorChecker::pCharacteristicNotify->addDescriptor(new BLE2902());


    //Vytvoření BLE komunikačního kanálu pro komunikaci
    BLECharacteristic *errorCharacteristicForce = pService->createCharacteristic(
        force_uuid,
        BLECharacteristic::PROPERTY_READ
    );
    //Nastavení zpětného volání pro danou charakteristiku                                                       
    errorCharacteristicForce->setCallbacks(new ErrorStateCharacteristicCallback());

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
    //Spuštění funkce pro zkontrolování chyb
    ErrorChecker::check_error();
}

//Funkce, která uvede chytrou ledničku do chybného režimu
void ErrorChecker::error_mode() {
    //Nastavení statické proměnné pro určování zda je lednička v chybě na log1
    ErrorChecker::fridge_error = true;
    //Pokud je zapntué piezo při chybě provede se následující 
    if(ErrorChecker::buzzing_on_error) {
        //Spuštění piezo po dobu 10 sekund
        PiezoManager::time_beep(10);
    } 
}
//Funkce, která uvede chytrou ledničku do kritického režimu
void ErrorChecker::fatal_error_mode() {
    //Spuštění funkce pro uvedení chytré ledničky do chybného stavu
    ErrorChecker::error_mode();
    //Spuštění funkce pro pozastavení chytré ledničky
    PowerManager::pause_fridge();
}

//Funkce, která zkontroluje chyby
void ErrorChecker::check_error() {
    //Deklarace proměnné pro ukládání předchozího stavu
    const std::string last_error_log = ErrorChecker::error_log;

    //Pokud se získaná vnitřní, venkovní teplota nebo teplota chladiče rovná "nan" provede se následující 
    if(FridgeData.in_temp == "nan" || 
       FridgeData.out_temp == "nan" || 
       FridgeData.cooler_temp == "nan") {
        //Spuštění funkce pro uvedení chytré ledničky do chybného stavu
        ErrorChecker::error_mode();
    } 
    //Pokud se chladící ventilátory netočí a relátko pro ventilátory je sepnuté, provede se následující 
    else if (PWM_FAN && !cooling_fans_pwm.get_is_fan_running() && relay_cooling_fans->get_is_open()) {
        //Spuštění funkce, která uvede chytrou ledničku do kritického režimu pokud není nastavený vývojářský režim 
        if(!ErrorChecker::fridge_fatal_error && !DEV_MODE) ErrorChecker::fatal_error_mode();
        //Nastavení proměnné, která určuje, zda je lednička v kritické chybě na log1
        ErrorChecker::fridge_fatal_error = true;
        //Uložíme log0 do příslušné pozice v error logu
        error_log[3] = 48;
    } else { //Pokud je vše v pořádku provede se následující
        //Pokud je proměnné nastavená na log1 provede se následující
        if(ErrorChecker::fridge_error || ErrorChecker::fridge_fatal_error) {
            //Spuštění funkce pro vypnutí peizo
            PiezoManager::stop_beep();
            //Nastavení statické proměnné pro určování zda je lednička v chybě na log0
            ErrorChecker::fridge_error = false;
        } 
    }

    //Zde zapíšeme v error logu log0 do příslušné pozice, pokud platí podmínky
    if (FridgeData.in_temp == "nan") ErrorChecker::error_log[0] = 48;
    else ErrorChecker::error_log[0] = 49;
    if (FridgeData.out_temp == "nan") ErrorChecker::error_log[1] = 48;
    else ErrorChecker::error_log[1] = 49;
    if (FridgeData.cooler_temp == "nan") ErrorChecker::error_log[2] = 48;
    else ErrorChecker::error_log[2] = 49;

    //! Pro chybu ventilátorů je nutné restart zařízení...

    //Pokud aktuální stav není roven předchozímu provede se následující 
    if(last_error_log != ErrorChecker::error_log) {
        //Vypsání hodnoty do konzole
        Serial.println("Odeslání informací o chybách");
        //Nastavení hodnoty charakteristiky 
        ErrorChecker::pCharacteristicNotify->setValue(ErrorChecker::error_log.c_str());
        //Odeslání zprávy skrze BLE do připojeného zařízení
        ErrorChecker::pCharacteristicNotify->notify();
    }
}

//Funkce, která vrátí, zda se lednička nachází v kritické chybě
bool ErrorChecker::is_fridge_on_fatal_error() {
    return ErrorChecker::fridge_fatal_error;
}