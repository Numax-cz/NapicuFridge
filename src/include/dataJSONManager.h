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
    //Deklarace statické proměnné pro uložení aktuální doby 
    static inline long time_now = 0;
    //Deklarace statické proměnné BLE komunikačního kanálu pro odesílání dat
    static inline BLECharacteristic *pCharacteristic = NULL;
    //Deklarace statické proměnné, která určuje zda jsou data připravená k odeslání do připojeného zařízení
    static inline bool ready_to_send = false;
    //Deklarace statické funkce pro zápis dat do souboru
    static void write();
public:
    /**
     * @brief Deklarace statické funkce pro inicializaci DataJsonManager 
     * 
     * @param pService BLE služba
     * @param notify_uuid UUID pro oznamování naměřených dat
     * @param ready_to_send_uuid UUID pro vynucení odeslání dat
     * @param delete_data_uuid UUID pro smazání naměřených hodnot
     */
    static void begin(BLEService* pService, const char* notify_uuid, const char* ready_to_send_uuid, const char* delete_data_uuid);
    //Deklarace statické loop funkce pro DataJSONManager
    static void loop();
    //Deklarace statické funkce, která odešle naměřená data do připojeného zařízení
    static void send();
    //Deklarace statické funkce, která nastaví proměnnou, která určuje zda jsou data připravená k odeslání do připojeného zařízení na log1
    static void set_ready_to_send() {DataJSONManager::ready_to_send = true;}
    //Deklarace statické funkce pro smazání souboru ukládající naměřené hodnoty
    static void delete_file();
};

//Deklarace třídy pro získání naměřených hodnot 
class DataJSONReadySendCharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic);
};

//Deklarace třídy pro smazání naměřených hodnot
class DataJSONDeleteCharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic);
};
