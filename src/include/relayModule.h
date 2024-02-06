#pragma once
#include "main.h"

class RelayModule {
private:
    //Deklarace proměnné pro uložení pinu relátka
    int pin;
    //Deklarace proměnné pro uložení zda je relátko otevřené
    bool is_open;
public:
    /**
     * @brief Deklarace konstruktoru třídy
     * 
     * @param pin Pin relátka 
     */
    RelayModule(int pin);
    //Deklarace funkce pro inicializace relátka
    void begin();
    //Deklarace funkce pro otevření relátka
    void open();
    //Deklarace funkce pro zavření relátka
    void close();
    //Deklarace funkce, která vrátí zda je relátko otevřené
    bool get_is_open() {return this->is_open;};
};