#pragma once
#include "main.h"


class RelayModule {
private:
    //Pin pro ovládání relátka
    int pin;

    //Proměnná pro uložení zda je relátko otevřeno
    bool is_open;
public:
    //Constructor
    RelayModule(int pin);
    //Definice funkce pro inicializace relátka
    void begin();
    //Definice funkce pro otevření relátka
    void open();
    //Definice funkce pro zavření relátka
    void close();
    //Funkce, která vrátí zda je relátko otevřeno
    bool get_is_open() {return this->is_open;};
};



