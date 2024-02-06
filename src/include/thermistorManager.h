/**
 * @file thermistorManager.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-15-10
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"

class ThermistorManager
{
private:
    //Deklarace ukazovací proměnné pro ukládání třídy termistoru
    Thermistor* ntc_thermistor;
    //Deklarace proměnné BLE komunikačního kanálu pro odesílání dat
    BLECharacteristic *pCharacteristic;
    //Deklarace referenční proměnné pro ukládání teploty 
    String& value;
public:
    /**
     * @brief Deklarace konstruktoru třídy
     *
     * @param pin Pin pro získávání dat z termistoru
     * @param uuid UUID pro odesílání dat
     * @param pService BLE služba
     * @param value Reference hodnoty na kterou se budou ukládat získané data
     * @param referenceResistance Referenční hodnota rezistoru 
     * @param nominalResistance  Nominální odpor
     * @param nominalTemperatureCelsius Nominální teplota 
     * @param bValue Beta hodnota
     * @param adcResolution Analogové rozlišení
     */
    ThermistorManager(int pin, const char* uuid, BLEService* pService, String& value, double referenceResistance, double nominalResistance, double nominalTemperatureCelsius, double bValue, int adcResolution = 1023);
    //Deklarace dekonstruktor pro třídu 
    ~ThermistorManager();
    //Deklarace funkce, která pošle data skrze BLE do připojeného zařízení
    void sendTemperature();
    //Deklarace funkce pro aktualizování hodnoty
    void updateTemperature();
};

