/**
 * @file errorChecker.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"

class ErrorChecker
{
private:
    //Deklarace statické proměnné BLE komunikačního kanálu pro odesílání dat
    static inline BLECharacteristic *pCharacteristic = NULL;
    //Deklarace statické proměnné která určuje, zda je lednička v chybě 
    static inline bool fridge_error = false;
    //Deklarace statické proměnné která určuje, zda je lednička v kritické chybě
    static inline bool fridge_fatal_error = false;
    //Deklarace funkce, která uvede chytrou ledničku do chybného režimu
    static void error_mode();
    //Deklarace funkce, která uvede chytrou ledničku do kritického režimu
    static void fatal_error_mode();
    //Deklarace funkce, která zkontroluje chyby
    static void check_error();
    //Deklarace statické proměnné, která uchovává předchozí stav
    static inline std::string error_log = "1111";
public:
    //Deklarace statické proměnné, která určuje, zda se má spustit piezo při chybě
    static inline bool buzzing_on_error = false;
    //Deklarace statické funkce pro inicializaci 
    static void begin(BLEService* pService, const char* notify_uuid);
    //Deklarace statické loop funkce pro ErrorChecker
    static void loop();
    //Deklarace statické funkce, která vrátí, zda se lednička nachází v kritické chybě
    static bool is_fridge_on_fatal_error();
};

//Deklarace třída pro správu piezo při erroru
class BuzzingOnErrorCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};


class ErrorStateCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};