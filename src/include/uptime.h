/**
 * @file uptime.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-16-10
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"


class FridgeUpTimeCharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onRead(BLECharacteristic *pCharacteristic);
};

