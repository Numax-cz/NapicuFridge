#pragma once
#include "main.h"

class ButtonManager
{
private:
    //Proměnná pro uložení pinu tlačítka    
    int pin;
    //Proměnná pro uložení stavu pinu tlačítka
    int buttonState = 0;

    //Čas, kdy bylo tlačítko naposledy stisknuto
    unsigned long buttonPressTime = 0; 
    //Proměnná, zda je tlačítko stisknuto
    bool buttonPressed = false; 
    //Proměnná pro sledování, zda byla funkce spuštěna
    bool functionExecuted = false; 



public:
    //Constructor
    ButtonManager(int pin);
    //Funkce pro inicializaci tlačítka
    void begin();
    //Funkce, která se spouští při každém loopu
    void loop();


    bool is_pressed();

    void button_hold_time(int time_ms, void (*callback)());
};


