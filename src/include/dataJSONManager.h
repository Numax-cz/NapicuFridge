/**
 * @file dataJSONManager.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-18-10
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"
//Připojení dalších potřebných knihoven
#include <ArduinoJson.h>
#include <SPIFFS.h>

class DataJSONManager
{
private:
    //Statická proměnná BLE komunikačního kanálu pro odesílání dat
    static inline BLECharacteristic *pCharacteristic = NULL;
    //Statická funkce pro zápis dat do souboru
    static void write();
public:
    //Statická funkce pro inicializaci DataJsonManager
    static void begin(BLEService* pService);

    //Statická loop funkce pro DataJSONManager
    static void loop();
};


