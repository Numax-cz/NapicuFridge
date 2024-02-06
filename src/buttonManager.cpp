#include <include/buttonManager.h>

//Konstruktor třídy 
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

//Funkce, která vrátí pokud je tlačítko drženo 
bool ButtonManager::is_pressed() {
    return this->pin == HIGH;
}

/**
  * @brief Funkce, která spustí funkci po určité době držení tlačítka 
  * 
  * @param time_ms Doba po kterou se tlačítko musí držet pro spuštění funkce (čas v ms)
  * @param callback Zpětná funkce 
  */
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