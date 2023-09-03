#include <include/buttonManager.h>

//Constructor
ButtonManager::ButtonManager(int pin) {
    //Uložení pinu z parametru constructoru do privátní proměnné
    this->pin = pin;
}

//Funkce pro inicializaci tlačítka
void ButtonManager::begin() {
    //Nastavení pinu na INPUT
    pinMode(pin, INPUT);
}

//Funkce, která se spouští při každém loopu
void ButtonManager::loop() {
    //Přečteme a následně uložíme hodnotu
    this->buttonState = digitalRead(this->pin);
}

bool ButtonManager::is_pressed() {
    return this->pin == HIGH;
}


void ButtonManager::button_hold_time(int time_ms, void (*callback)()) {
  // Pokud je tlačítko stisknuto a funkce nebyla spuštěna 
  if (buttonState == HIGH && !this->functionExecuted) {
    if (!buttonPressed) {
      // Uložení času, kdy bylo tlačítko stisknuto poprvé
      buttonPressTime = millis(); 
      // Nastavení proměnné, že je tlačítko stisknuto
      buttonPressed = true; 
    }
    // Pokud bylo tlačítko stisknuto po dobu 5 sekund
    if (millis() - buttonPressTime >= 5000) {
      // Spuštění funkce z parametru 
      callback();
      // Můžete zde přidat kód pro spuštění vaší funkce
      this->functionExecuted = true;
    }
  } else {
    buttonPressed = false; // Resetovat příznak stisknutého tlačítka, když není stisknuto
    this->functionExecuted = false;
  }
}