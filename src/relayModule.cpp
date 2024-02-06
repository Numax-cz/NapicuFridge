#include <include/relayModule.h>

/**
* @brief Deklarace konstruktoru třídy
* 
* @param pin Pin relátka 
*/
RelayModule::RelayModule(int pin) {
    this->pin = pin;
}

//Funkce pro inicializaci relátek
void RelayModule::begin() {
    //Nastavení pinu output
    pinMode(this->pin, OUTPUT);
    //Spuštění funkce pro zavření relátka
    this->close();
}

//Funkce pro otevření relátka
void RelayModule::open() {
    //Spuštění funkce pro nastavení pinu na low
    digitalWrite(this->pin, LOW);
    this->is_open = true;
}

//Funkce pro zavření relátka
void RelayModule::close() {
    //Spuštění funkce pro nastavení pinu na high
    digitalWrite(this->pin, HIGH);
    this->is_open = false;
}
