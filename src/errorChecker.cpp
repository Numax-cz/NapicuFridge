#include <include/errorChecker.h>

//Loop funkce pro ErrorChecker
void ErrorChecker::loop() {
    //Pokud se získaná vnitřní nebo venkovní teplota rovná "nan" provede se následující 
    if(FridgeData.in_temp == "nan" || 
       FridgeData.out_temp == "nan") {
        PiezoManager::time_beep(60);
        return;
    } else {
        PiezoManager::stop_beep();
    }


}