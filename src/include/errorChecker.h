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
    //Deklarace statické proměnné která určuje, zda je lednička v chybě 
    static inline bool fridge_error = false;
    //Deklarace statické proměnné která určuje, zda je lednička v kritické chybě
    static inline bool fridge_fatal_error = false;
    //Deklarace funkce, která zkontroluje chyby
    static void check_error();
public:
    //Deklarace statické proměnné, která určuje, zda se má spustit piezo při chybě
    static inline bool buzzing_on_error = false;
    //Deklarace statické funkce pro inicializaci 
    static void begin();
    //Deklarace statické loop funkce pro ErrorChecker
    static void loop();
};

//Deklarace třída pro správu piezo při erroru
class BuzzingOnErrorCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};

