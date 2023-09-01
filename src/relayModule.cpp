#include <include/relayModule.h>

//Constructor
RelayModule::RelayModule(int pin) {
    this->pin = pin;
}

//Funkce pro inicializaci relátek
void RelayModule::begin() {
    pinMode(this->pin, OUTPUT);
}

//Funkce pro otevření relátka
void RelayModule::open() {
    digitalWrite(this->pin, HIGH);
    this->is_open = true;
}

//Funkce pro zavření relátka
void RelayModule::close() {
    pinMode(this->pin, LOW);
    this->is_open = false;
}