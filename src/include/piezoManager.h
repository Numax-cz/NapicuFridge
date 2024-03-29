/**
 * @file piezoManager.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-13-10
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"

class PiezoManager
{
private:
    //Deklarace statické proměnné, která určuje, zda piezo je aktivní a bzučí     
    static inline bool piezo_buzzing = false;
    //Deklarace statické proměnné, která ukládá počet zabzučení piezo
    static inline int piezo_tone_count_now = 0;
    //Deklarace statické proměnné aktuální doby 
    static inline long time_now = 0;
    //Deklarace konstantní statické proměnné, která určuje zpoždění mezi jednotlivými zabzučenímy a délku bzučení
    static const inline int delay = 250; //V našem případě 250ms 
    //Deklarace proměnné pro uložení zpětného volání (pokud je tato funkce nastavená, zavolá se vždy, když pípání skončí)
    static inline void (*callback)() = NULL;
    //Deklarace statické proměnné, která určuje, kolikrát má peizo zabzučet
    static inline int piezo_tone_count = 0;
public:
    //Deklarace statické loop funkce pro PiezoManager
    static void loop();
    //Deklarace statické funkce, která zastaví pípání piezo
    static void stop_beep();
    /**
     * Deklarace statické funkce, která spustí bzučení piezo a zopakuje se tolikrát, kolik je definované v count parametru
     * @param count Počet opakování  
     * @param callback Funkce, která se spustí po ukončení počtu opakování
     */
    static void tone_beep(int count, void (*callback)());
    /**
     * Deklarace statické funkce, která spustí bzučení piezo a zopakuje se tolikrát, kolik je definované v count parametru
     * @param count Počet opakování  
     */
    static void tone_beep(int count);
    /**
     * Deklarace statické funkce, která spustí bzučení piezo po dobu definovanou v seconds parametru
     * @param seconds Doba po kterou má bzučák pípat (v sekundách)
     * @param callback Funkce, která se spustí po ukončení piezo pípání
     */
    static void time_beep(int seconds, void (*callback)());
    /**
     * Deklarace statické funkce, která spustí bzučení piezo po dobu definovanou v seconds parametru
     * @param seconds Doba po kterou má bzučák pípat (v sekundách)
     */
    static void time_beep(int seconds);
};