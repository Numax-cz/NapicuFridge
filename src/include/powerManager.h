
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

public: 

    //Begin funkce pro PowerManager
    static void begin();
    //Statická loop funkce pro PowerManager
    static void loop();
    //Statická funkce pro vypnutí celého chladícího systému
    static void power_off();
    //Statická funkce pro zapnutí celého chladícího systému
    static void power_on();
    //Statická funkce pro vypnutí chladících ventilátorů
    static void turn_off_cooling_fans();
    //Statická funkce pro zapnutí chladících ventilátorů
    static void turn_on_cooling_fans();
    //Statická funkce pro vypnutí vnitřních ventilátorů
    static void turn_off_in_fans();
    //Statická funkce pro zapnutí vnitřních ventilátorů
    static void turn_on_in_fans();

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