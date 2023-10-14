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
    //Statická proměnná která určuje, zda je lednička v chybě 
    static inline bool fridge_error = false;
    //Statická proměnná která určuje, zda je lednička v kritické chybě
    static inline bool fridge_fatal_error = false;
    //Statická funkce, která zkontroluje chyby
    static void check_error();
    //Statická funkce, která zkontroluje kritické chyby
    static void check_fatal_error();
public:
    //Statická proměnná, která určuje, zda se má spustit piezo při chybě
    static inline bool buzzing_on_error = false;
    //Statická funkce pro inicializaci 
    static void begin();

    //Statická loop funkce pro ErrorChecker
    static void loop();
};


//Třída pro správu piezo při erroru
class BuzzingOnErrorCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};

