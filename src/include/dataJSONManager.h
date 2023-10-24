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
    //Statická proměnná aktuální doby 
    static inline long time_now = 0;
    //Statická proměnná BLE komunikačního kanálu pro odesílání dat
    static inline BLECharacteristic *pCharacteristic = NULL;
    //Statická proměnná, která určuje zda jsou data připravená k odeslání do připojeného zařízení
    static inline bool ready_to_send = false;
    //Statická funkce pro zápis dat do souboru
    static void write();
public:
    /**
     * @brief Statická funkce pro inicializaci DataJsonManager 
     * 
     * @param pService BLE služba
     * @param notify_uuid UUID pro oznamování naměřených dat
     * @param ready_to_send_uuid UUID pro vynucení odeslání dat
     */
    static void begin(BLEService* pService, const char* notify_uuid, const char* ready_to_send_uuid);
    //Statická loop funkce pro DataJSONManager
    static void loop();
    //Statická funkce, která odešle naměřená data do připojeného zařízení
    static void send();
    //Statická funkce, která nastaví proměnnou, která určuje zda jsou data připravená k odeslání do připojeného zařízení na log1
    static void set_ready_to_send() {DataJSONManager::ready_to_send = true;}
};

class DataJSONReadySendCharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic);
};