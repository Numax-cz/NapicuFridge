#pragma once
#include "main.h"

class ButtonManager
{
private:
    //Deklarace proměnné pro uložení pinu tlačítka    
    int pin;
    //Deklarace proměnné pro uložení stavu pinu tlačítka
    int buttonState = 0;
    //Deklarace proměnné času, kdy bylo tlačítko naposledy stisknuto
    unsigned long buttonPressTime = 0; 
    //Deklarace proměnné, která určuje zda je tlačítko stisknuto
    bool buttonPressed = false; 
    //Deklarace proměnné pro sledování, zda byla funkce spuštěna
    bool functionExecuted = false; 
public:
    /**
     * @brief Deklarace konstruktoru třídy
     * 
     * @param pin Pin tlačítka
     */
    ButtonManager(int pin);
    //Deklarace funkce pro inicializaci tlačítka
    void begin();
    //Deklarace funkce, která se spouští při každém loopu
    void loop();
    //Deklarace funkce, která vrátí pokud je tlačítko drženo 
    bool is_pressed();
    /**
     * @brief Deklarace funkce, která spustí funkci po určité době držení tlačítka 
     * 
     * @param time_ms Doba po kterou se tlačítko musí držet pro spuštění funkce (čas v ms)
     * @param callback Zpětná funkce 
     */
    void button_hold_time(int time_ms, void (*callback)());
};


