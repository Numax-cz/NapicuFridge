/**
 * @file fridgeFactoryReset.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-29-10
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"

class FridgeFactoryReset {
public: 
    //Statická funkce, která uvede zařízení do továrního nastavení 
    static void factory_reset();
};

//Třída pro správu displeje chytré ledničky
class FactoryResetCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
};