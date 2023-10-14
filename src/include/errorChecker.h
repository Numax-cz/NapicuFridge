/**
 * @file errorChecker.h
 * @author Marcel Mikoláš
 * @version 0.1
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
*/

//Zabráníme několikanásobnému zahrnutí knihoven 
#pragma once
//Připojení main knihovny
#include "main.h"

class ErrorChecker
{
private:
    //Proměnná která určuje, zda je lednička v chybě 
    static inline bool fridge_error = false;
    //Proměnná která určuje, zda je lednička v kritické chybě
    static inline bool fridge_fatal_error  = false;

public:
    //Statická loop funkce pro ErrorChecker
    static void loop();


};


