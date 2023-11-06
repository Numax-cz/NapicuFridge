
/**
 * @file powerManager.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-01-10
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"
//Předávací deklarace třídy RelayModule
class RelayModule;
//Předávací deklarace třídy FanController
template <uint8_t pwm_pin, uint8_t tach_pin>
class FanController;

class PowerManager {

protected: 
    //Statická proměnná BLE komunikačního kanálu pro odesílání dat
    static inline BLECharacteristic *pCharacteristic = NULL;
    //Statická proměnná pro ukládání zda jsou dveře otevřeny
    static inline bool is_door_open = false;
    //Statická funkce pro inicializaci vnitřních ventilátorů
    static void begin_in_fans();
    //Statická funkce pro vypnutí chladících ventilátorů
    static void turn_off_cooling_fans();
    //Statická funkce pro zapnutí chladících ventilátorů
    static void turn_on_cooling_fans();
    //Statická funkce pro vypnutí celého chladícího systému
    static void power_off();
    //Statická funkce, která načte veškerá nastavení z EEPROM
    static void load_config_from_eeprom();
public: 
    //Statická proměnná, která určuje zda se lednička pozastaví při otevřených dveří 
    static inline int fridge_pause_on_door_open = -1;
    //Begin funkce pro PowerManager
    static void begin();
    //Statická loop funkce pro PowerManager
    static void loop();
    //Statická funkce pro zapnutí celého chladícího systému
    static void power_on();
    //Statická funkce pro změnu napájecího režimu
    static void change_power_mode(int mode);
    //Statická funkce pro vypnutí vnitřních ventilátorů
    static void turn_off_in_fans();
    //Statická funkce pro zapnutí vnitřních ventilátorů
    static void turn_on_in_fans();
    //Statická funkce, která vrací zda je chladící systém zapnutý 
    static bool is_power_on();
};

class PowerManagerCharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};


class InFansCharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};

class DoorCharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};