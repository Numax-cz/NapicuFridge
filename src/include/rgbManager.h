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
// //Připojení main knihovny
// #include "main.h"
//Připojení potřebných knihoven 
#include <Adafruit_NeoPixel.h>


class RGBManager
{
private:
    //Proměnná pro uložení třídy RGB světla 
    Adafruit_NeoPixel* rgbWS = NULL;
    //Proměnná pro uložení aktuálního nastavení RGB
    uint32_t rgb = 0;
    //Konstantní proměnná pro uložení počtů LED
    const uint16_t leds;
public:
    /**
     * @brief Konstruktor pro vytvoření nové třídy pro RGB světlo 
     * 
     * @param leds Počet led 
     * @param pin Pin RGB 
     */
    RGBManager(uint16_t leds, int16_t pin);
    //Definice funkce pro inicializace RGB světla
    void begin();
    //Definice loop funkce pro RGB světlo
    void loop();
    /**
     * @brief Definice funkce pro nastavení RGB 
     * 
     * @param r Červená (0-255) 
     * @param g Zelená (0-255) 
     * @param b Modrá (0-255) 
     */
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    /**
     * @brief Definice funkce pro nastavení jasu RGB světla
     * 
     * @param brightness Hodnota jasu (0-255)
     */
    void setBrightness(uint8_t brightness);
    //Destruktor pro RGB světlo
    ~RGBManager();
};

