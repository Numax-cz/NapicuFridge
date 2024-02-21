#include <include/piezoManager.h>

//Statická loop funkce pro PiezoManager
void PiezoManager::loop() {
    //Uložíme aktuální čas běhu do konstantní proměnné time 
    const unsigned long time = millis();
    if(PiezoManager::piezo_buzzing) {
        //Načasování programu, každou dobu určující statická proměnná delay se provede následující
        if(time >= PiezoManager::time_now + PiezoManager::delay) {
            //Přičte se statická proměnná delay k proměnné určující aktuální dobu
            PiezoManager::time_now += PiezoManager::delay;
            //Přičte se 1 k statické proměnné, která počítá pípání piezo
            PiezoManager::piezo_tone_count_now++;

            //Deklarace a uložení hodnoty z reset piezo pinu
            int pin_value = digitalRead(PIEZO_PIN);

            //Zapsání log1, nebo log0 dle následující podmínky do reset piezo pinu
            digitalWrite(PIEZO_PIN, !pin_value ? 1 : 0);
            //Pokud je po přičtení 1 piezo_tone_count_now větší než statická proměnná piezo_tone_count provede se následující
            if(PiezoManager::piezo_tone_count_now >= PiezoManager::piezo_tone_count) {
                //Pokud je deklarované zpětné volání, spustí se funkce
                if(PiezoManager::callback != NULL) PiezoManager::callback();
                //Spuštění funkce pro vynulování a zastavení piezo
                PiezoManager::stop_beep();
            }
        }
    } else {
        //Nastavení statické proměnné určující aktuální období
        PiezoManager::time_now = time;
    }
}

//Statická funkce, která zastaví pípání piezo
void PiezoManager::stop_beep() {
        //Vynulování statické proměnné pro počítání (nastavíme proměnnou na log0)
    PiezoManager::piezo_tone_count_now = 0;
    //Nastavení statické proměnné na log0
    PiezoManager::piezo_buzzing = false;
    //Nastavení statické proměnné zpětného volání na NULL
    PiezoManager::callback = NULL;
    //Zapsání log0 na piezo pin
    digitalWrite(PIEZO_PIN, LOW);
}

/**
 * Statická funkce, která spustí bzučení piezo a zopakuje se tolikrát, kolik je definované v count parametru
 * @param count Počet opakování  
 * @param callback Funkce, která se spustí po ukončení počtu opakování
 */
void PiezoManager::tone_beep(int count, void (*callback)()) {
    //Pokud je bzučení neaktivní provede se následující 
    if(!PiezoManager::piezo_buzzing) {
        //Nastaví se statická proměnná pro zpětné volání
        PiezoManager::callback = callback;
        //Nastaví se proměnná pro počet zabzučení piezo včetně mezer mezi zabzučením
        PiezoManager::piezo_tone_count = (count * 2);
        //Nastavení proměnné na log1
        PiezoManager::piezo_buzzing = true;
    } 
}

/**
 * Statická funkce, která spustí bzučení piezo a zopakuje se tolikrát, kolik je definované v count parametru
 * @param count Počet opakování  
 */
void PiezoManager::tone_beep(int count) {
    //Spuštění funkce s parametrem callback NULL
    PiezoManager::tone_beep(count, NULL);
}

/**
 * Statická funkce, která spustí bzučení piezo po dobu definovanou v seconds parametru
 * @param seconds Doba po kterou má bzučák pípat (v sekundách)
 * @param callback Funkce, která se spustí po ukončení piezo pípání
 */
void PiezoManager::time_beep(int seconds, void (*callback)()) {
     //Pokud je bzučení neaktivní provede se následující 
    if(!PiezoManager::piezo_buzzing) {
        //Nastaví se statická proměnná pro zpětné volání
        PiezoManager::callback = callback;
        //Nastaví se proměnná pro počet zabzučení piezo včetně mezer mezi zabzučením
        PiezoManager::piezo_tone_count = (seconds * 1000) / PiezoManager::delay;
        //Nastavení proměnné na log1
        PiezoManager::piezo_buzzing = true;
    }   
}

/**
 * Statická funkce, která spustí bzučení piezo po dobu definovanou v seconds parametru
 * @param seconds Doba po kterou má bzučák pípat (v sekundách)
 */
void PiezoManager::time_beep(int seconds) {
    //Spuštění funkce s parametrem callback NULL
    PiezoManager::time_beep(seconds, NULL);
}
