/**
 * @file rgbManager.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-2-12
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"
//Připojení potřebných knihoven 
#include <Adafruit_NeoPixel.h>

class RGBManager
{
private:
    //Deklarace proměnné pro uložení třídy RGB světla 
    Adafruit_NeoPixel* rgbWS = NULL;
    //Deklarace proměnné pro uložení aktuálního nastavení RGB
    uint32_t rgb = 0;
    //Deklarace proměnné pro uložení aktuální hodnoty jasu 
    uint8_t brightness = 0;
    //Deklarace konstantní proměnné pro uložení počtů LED
    const uint16_t leds;
public:
    /**
     * @brief Deklarace konstruktor třídy 
     * 
     * @param leds Počet led 
     * @param pin Pin RGB 
     */
    RGBManager(uint16_t leds, int16_t pin);
    //Deklarace funkce pro inicializaci RGB světla
    void begin();
    //Deklarace loop funkce pro RGB světlo
    void loop();
    //Deklarace funkce pro zapnutí RGB světla
    void turn_on();
    //Deklarace funkce pro vypnutí RGB světla
    void turn_off();
    /**
     * @brief Deklarace funkce pro nastavení RGB 
     * 
     * @param r Hodnota červená (0-255) 
     * @param g Hodnota zelená (0-255) 
     * @param b Hodonta modrá (0-255) 
     */
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    /**
     * @brief Deklarace funkce pro nastavení jasu RGB světla
     * 
     * @param brightness Hodnota jasu (0-100)
     */
    void setBrightness(uint8_t brightness);
    //Destruktor pro RGB světlo
    ~RGBManager();
};

//Deklarace třídy pro vypnutí/zapnutí RGB osvětlení při otevření dveří
class RGBEnableCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};
//Deklarace třídy pro změnu barvy RGB osvětlení
class RGBColorCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};
//Deklarace třídy pro změnu jasu RGB osvětlení
class RGBBrightnessCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};

