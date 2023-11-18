
/**
 * @file digitalPotenciometer.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-18-11
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"


class DigitalPotentiometer {
private:
    //Proměnná, která uchovává třídu digitálního potenciometru
    DigiPot* potentiometer = NULL;

public:
    /**
     * @brief Konstruktor pro vytvoření nové třídy digitálního potenciometru
     * 
     * @param incPin Pin INC pinu potenciometru
     * @param udPin Pin UD pinu potenciometru 
     * @param csPin Pin CS pinu potenciometru
     */
    DigitalPotentiometer(uint8_t incPin, uint8_t udPin, uint8_t csPin);
    //Dekonstruktor pro třídu digitálního potenciometru
    ~DigitalPotentiometer();
    /**
     * @brief Funkce, která nastaví hodnotu potenciometru
     * 
     * @param value Hodnota v procentech 
     */
    void set(uint8_t value);
};