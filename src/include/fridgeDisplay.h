#pragma once
#include "main.h"

//Definice šířky displeje v px
#define DISPLAY_W 128
//Definice výšky displeje v px
#define DISPLAY_H 32

//Definice stavů displeje
typedef enum {
    FRIDGE_DISPLAY_PAIR_TEXT = 0, //Tento stav vypíše párovací text
    FRIDGE_DISPLAY_IN_TEMP_1, //Tento stav vypíše vnitřní teplotu
    FRIDGE_DISPLAY_OUT_TEMP_1, //Tento stav vypíše venkovní teplotu
    FRIDGE_DISPLAY_COOLER_TEMP,  //Tento stav vypíše teplotu chladiče
} fridge_display_state;

class FridgeDisplay {
private:
    //Definice statické proměnná pro uložení třídy pro displej
    static inline Adafruit_SSD1306* display = NULL;
    //Definice statické proměnná pro uložení stavu displeje
    static inline fridge_display_state display_state = FRIDGE_DISPLAY_PAIR_TEXT; 
    //Definice statické proměnná pro uložení zda je displej povolen
    static inline bool is_dislay_enable = true;
    /**
     * @brief Definice statické funkce, která vypíše text vycentrovaně
     * 
     * @param text Text, který se má zobrazit 
     * @param font_size Velikost textu
     * @param x Pozice x textu
     */
    void static print_centered_text(String text, uint8_t font_size = 1, int16_t x = -1);
    //Definice statické proměnné pro uložení aktuálního zobrazovaného textu
    static inline char message[]="Sparujte zarizeni";
    //Definice statických proměn pro uložení pozic
    static inline int x, minX;
    //Definice statické funkce pro vypsání textu pro párování 
    void static print_pair_text();
public:
    //Definice statické funkce pro inicializaci displeje
    void static begin();
    //Definice statické funkce pro aktualizaci 
    void static loop();
    /**
     * @brief Definice statické funkce pro změnu stavu displeje
     * 
     * @param state Nový stav displeje 
     */
    void static change_display_state(fridge_display_state state) {FridgeDisplay::display_state = state;}
    //Definice statické funkce pro vrácení, zda je displej dostupný
    int static get_is_enable() {return FridgeDisplay::is_dislay_enable;}
    //Definice statické funkce pro vrácení aktuálního stavu displeje
    fridge_display_state static get_display_state() {return FridgeDisplay::display_state;}
    //Definice statické funkce pro povolení displeje
    void static enable_display();
    //Definice statické funkce pro zakázání displeje
    void static disable_display();
};

//Třída pro správu displeje chytré ledničky
class DisplayEnableCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};

//Třída pro správu stavů displeje chytré ledničky
class DisplayStateCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};