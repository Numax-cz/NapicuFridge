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

    //Proměnná pro uložení hodnoty červené barvy
    uint8_t R = EEPROM.read(LED_COLOR_EEPROM_ADDR);
    //Proměnná pro uložení hodnoty zelené barvy
    uint8_t G = EEPROM.read(LED_COLOR_EEPROM_ADDR + 1);
    //Proměnná pro uložení hodnoty modré barvy
    uint8_t B = EEPROM.read(LED_COLOR_EEPROM_ADDR + 2);
    //Proměnná pro uložení hodnoty jasu
    uint8_t Brightness = EEPROM.read(LED_BRIGHTNESS_EEPROM_ADDR);

    //Pokud se hodnota rovná 255 (neexistuje) provede se následující 
    if(Brightness == 0xFF) {
        //Nastavení výchozí hodnoty
        Brightness = DEFAULT_LED_BRIGHTNESS;
    }

    //Spuštění funkce pro nastavení barvy LED osvětlení 
    fridge_rgb->setColor(R, G, B);

    //Spuštění funkce pro nastavení jasu LED osvětlení 
    fridge_rgb->setBrightness(Brightness);
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
    //Spuštění funkce pro nastavení jasu
    this->rgbWS->setBrightness(this->brightness);
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
 * @param brightness Hodnota jasu (0-100)
 */
void RGBManager::setBrightness(uint8_t brightness) {
    this->brightness = 2.55 * brightness;
}

//Destruktor pro RGB světlo
RGBManager::~RGBManager() {
    //Smazání proměnné z paměti 
    delete this->rgbWS;
}