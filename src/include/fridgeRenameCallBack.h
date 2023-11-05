/**
 * @file fridgeNameCallBack.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-1-11
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"

class FridgeRenameCallBack : public BLECharacteristicCallbacks { 
public:
    void onWrite(BLECharacteristic *pCharacteristic);
};