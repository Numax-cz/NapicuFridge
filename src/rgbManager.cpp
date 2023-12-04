/**
 * @file rgbManager.cpp
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-2-12
 * 
 * @copyright Copyright (c) 2023
*/

//Připojení hlavní knihovny 
#include <include/rgbManager.h>

/**
 * @brief Konstruktor pro vytvoření nové třídy pro RGB světlo 
 * 
 * @param leds Počet led 
 * @param pin Pin RGB 
 */
RGBManager::RGBManager(uint16_t leds, int16_t pin) : leds(leds) {
    this->rgbWS = new Adafruit_NeoPixel(leds, pin, NEO_GRB + NEO_KHZ800);
}

//Funkce pro inicializace RGB světla
void RGBManager::begin() {
    this->rgbWS->begin();
    //Spuštění funkce pro RGB světla
    this->turn_off();
}

//Loop funkce pro RGB světlo
void RGBManager::loop() {
    //Loop, který nastavý barvu všem LED diodám 
    for (int i = 0; i < this->leds; i++) {
        //Nastavení barvy pro danou LED diodu podle indexu (i)
        this->rgbWS->setPixelColor(i, rgb);
    }
    //Spuštění funkce pro aktualizaci barev na všech modulech
    this->rgbWS->show();
}

//Definice funkce pro zapnutí RGB světla
void RGBManager::turn_on() {
    this->rgbWS->setBrightness(255); //TODO BY EEprom + documentation
}

//Definice funkce pro vypnutí RGB světla
void RGBManager::turn_off() {
    this->rgbWS->setBrightness(0);
}

/**
 * @brief Funkce pro nastavení RGB 
 * 
 * @param r Červená (0-255) 
 * @param g Zelená (0-255) 
 * @param b Modrá (0-255) 
 */
void RGBManager::setColor(uint8_t r, uint8_t g, uint8_t b) {
    //Spuštění funkce pro nastavení RGB
    this->rgb = this->rgbWS->Color(r, g, b);
}

/**
 * @brief Definice funkce pro nastavení jasu RGB světla
 * 
 * @param brightness Hodnota jasu (0-255)
 */
void RGBManager::setBrightness(uint8_t brightness) {
    //Spuštění funkce pro nastavení jasu
    this->rgbWS->setBrightness(brightness);
}

//Destruktor pro RGB světlo
RGBManager::~RGBManager() {
    //Smazání proměnné z paměti 
    delete this->rgbWS;
}