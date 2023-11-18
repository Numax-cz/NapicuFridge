#include <include/digitalPotentiometer.h>

/**
 * @brief Konstruktor pro vytvoření nové třídy digitálního potenciometru
 * 
 * @param incPin Pin INC pinu potenciometru
 * @param udPin Pin UD pinu potenciometru 
 * @param csPin Pin CS pinu potenciometru
 */
DigitalPotentiometer::DigitalPotentiometer(uint8_t incPin, uint8_t udPin, uint8_t csPin) {
    this->potentiometer = new DigiPot(incPin, udPin, csPin);
}

//Dekonstruktor pro třídu digitálního potenciometru
DigitalPotentiometer::~DigitalPotentiometer() {
    delete this->potentiometer;
}

/**
 * @brief Funkce, která nastaví hodnotu potenciometru
 * 
 * @param value Hodnota v procentech 
 */
void DigitalPotentiometer::set(uint8_t value) {
    this->potentiometer->set(value);
}