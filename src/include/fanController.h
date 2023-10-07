
/**
 * @file fanController.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-29-09
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"

/**
 * @brief 
 * 
 * @tparam pwm_pin Ve většině případů modrý vodič u ventilátoru řízené PWM
 * @tparam tach_pin Ve většině případů zelený vodič u ventilátoru řízené PWM
 */
template <uint8_t pwm_pin, uint8_t tach_pin>
class   FanController
{
private:
    //Statická proměnná pro ukládání počtů záznamů 
    static int count;
    //Statická proměnná doby, po kterou se má čekat mezi kontrolou
    static const int fan_data_period;
    //Statická proměnná aktuální doby v kontrole
    static unsigned long fan_time_now;
    //Statická proměnná pro ukládání zda ventilátor běží
    static bool running;
    //Funkce pro počítání 
    static void counter() {
        //Přičteme ke statické proměnné 1 
        FanController<pwm_pin, tach_pin>::count++;
    }
public:
    //Statická funkce pro inicializaci ventilátoru
    void static begin() {
        //Nastavíme pin mode
        pinMode(pwm_pin, OUTPUT);
        //Připojíme statickou funkci (counter) k požadovanému pinu - funkce se vyvolá na vzestupné hraně signálu  (když se pin stane HIGH)
        attachInterrupt(digitalPinToInterrupt(tach_pin), counter, RISING);
        //Nastavení výchozí hodnoty na 100%
        analogWrite(pwm_pin, 255);
    }
    //Statická funkce, která se spouští při každém loopu
    void static loop() {
        //Uložíme aktuální čas běhu do konstantní proměnné time 
        const unsigned long time = millis();
        //Načasování programu
        if(time >= FanController::fan_time_now + FanController::fan_data_period) {
            //Přičtení kontrolní doby k aktuálnímu času 
            FanController::fan_time_now += FanController::fan_data_period;
            //Nastavení proměnné podle statické proměnné count po logickém negování 
            FanController::running = !!FanController::count;
            //Nastavení proměnné zpět na hodnotu 0
            FanController::count = 0;
        }
    }
    //Statická funkce, která nastaví ventilátory na požadované %
    void static set_fan_speed_in_percentage(int percentage) {
        //Zapíšeme hodnotu do požadovaného pinu
        analogWrite(pwm_pin, (255 / 100) * percentage);
    }
    //Statická funkce, která vrátí zda se ventilátor točí
    bool static get_is_fan_running() {
        //Zapíšeme hodnotu do požadovaného pinu
        return FanController::running;
    }
};

//Inicializace statických proměnn

//Statická proměnná pro ukládání počtů záznamů 
template <uint8_t pwm_pin, uint8_t tach_pin>
int FanController<pwm_pin, tach_pin>::count = 0;

//Statická proměnná doby, po kterou se má čekat mezi kontrolou
template <uint8_t pwm_pin, uint8_t tach_pin>
const int FanController<pwm_pin, tach_pin>::fan_data_period = 1000;

//Statická proměnná aktuální doby v kontrole
template <uint8_t pwm_pin, uint8_t tach_pin>
unsigned long FanController<pwm_pin, tach_pin>::fan_time_now = 0;

//Statická proměnná pro ukládání zda ventilátor běží
template <uint8_t pwm_pin, uint8_t tach_pin>
bool FanController<pwm_pin, tach_pin>::running = true;

