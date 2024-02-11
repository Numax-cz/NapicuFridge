
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
    //Deklarace statické proměnné BLE komunikačního kanálu pro odesílání dat
    static inline BLECharacteristic *pCharacteristic = NULL;
    //Deklarace statické proměnné pro ukládání zda jsou dveře otevřeny
    static inline bool is_door_open = false;
    //Deklarace statické proměnné, která ukládá aktuální zvolený napájecí režim 
    static inline int selected_mode = -1;
    //Deklarace statické funkce pro inicializaci vnitřních ventilátorů
    static void begin_in_fans();
    //Deklarace statické funkce pro vypnutí chladících ventilátorů
    static void turn_off_cooling_fans();
    //Deklarace statické funkce pro zapnutí chladících ventilátorů
    static void turn_on_cooling_fans();
    //Deklarace statické funkce pro vypnutí celého chladícího systému
    static void power_off();
    //Deklarace statické funkce, která načte veškerá nastavení z EEPROM
    static void load_config_from_eeprom();
    //Deklarace statické funkce, která pošle připojenému zařízení aktuální režim napájení + nastavení vnitřních ventilátorů
    static void notify_power_config();
public: 
    //Deklarace statické proměnné, která určuje zda se lednička pozastaví při otevřených dveří 
    static inline int fridge_pause_on_door_open = -1;
    //Deklarace statické proměnné, která určuje zda se má LED osvětlení zapnout při otevřených dveří
    static inline int fridge_led_enable_on_door_open = -1;
    /**
     * @brief Begin funkce pro PowerManager
     * 
     * @param pService BLE služba
     * @param notify_uuid UUID pro odesílání dat při změně režimu napájení 
     */
    static void begin(BLEService* pService, const char* notify_uuid);
    //Deklarace statické loop funkce pro PowerManager
    static void loop();
    //Deklarace statické funkce pro zapnutí celého chladícího systému
    static void power_on();
    /**
     * @brief Deklarace statické funkce pro změnu napájecího režimu
     * 
     * @param mode Režim napájení který se nastaví
     */
    static void change_power_mode(int mode);
    //Deklarace statické funkce pro vypnutí vnitřních ventilátorů
    static void turn_off_in_fans();
    //Deklarace statické funkce pro zapnutí vnitřních ventilátorů
    static void turn_on_in_fans();
    //Deklarace statické funkce, která vrací zda je chladící systém zapnutý 
    static bool is_power_on();
    //Deklarace statické funkce, která uvede ledničku do pauzy
    static void pause_fridge();
    //Deklarace statické funkce, která zruší pauzu ledničky
    static void cancel_pause_fridge();
};

//Deklarace třídy pro zápis/čtení informací pro správce napájení 
class PowerManagerCharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};
//Deklarace třídy pro zápis/čtení informací vnitřních ventilátorů 
class InFansCharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};
//Deklarace třídy pro zápis/čtení informací senzoru dveří 
class DoorCharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};