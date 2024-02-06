#pragma once
#include "main.h"

//Deklarace šířky displeje v px
#define DISPLAY_W 128
//Deklarace výšky displeje v px
#define DISPLAY_H 32

//Deklarace stavů displeje
typedef enum {
    FRIDGE_DISPLAY_PAIR_TEXT = 0, //Tento stav vypíše párovací text
    FRIDGE_DISPLAY_IN_TEMP_1, //Tento stav vypíše vnitřní teplotu
    FRIDGE_DISPLAY_OUT_TEMP_1, //Tento stav vypíše venkovní teplotu
    FRIDGE_DISPLAY_COOLER_TEMP,  //Tento stav vypíše teplotu chladiče
} fridge_display_state;

class FridgeDisplay {
private:
    //Deklarace statické proměnné pro uložení třídy pro displej
    static inline Adafruit_SSD1306* display = NULL;
    //Deklarace statické proměnné pro uložení stavu displeje
    static inline fridge_display_state display_state = FRIDGE_DISPLAY_PAIR_TEXT; 
    //Deklarace statické proměnné pro uložení zda je displej povolen
    static inline bool is_dislay_enable = true;
    /**
     * @brief Deklarace statické funkce, která vypíše text vycentrovaně
     * 
     * @param text Text, který se má zobrazit 
     * @param font_size Velikost textu
     * @param x Pozice x textu
     */
    void static print_centered_text(String text, uint8_t font_size = 1, int16_t x = -1);
    //Deklarace statické proměnné pro uložení aktuálního zobrazovaného textu
    static inline char message[]="Sparujte zarizeni";
    //Deklarace statických proměn pro uložení pozic
    static inline int x, minX;
    //Deklarace statické funkce pro vypsání textu pro párování 
    void static print_pair_text();
public:
    //Deklarace statické funkce pro inicializaci displeje
    void static begin();
    //Deklarace statické funkce pro aktualizaci 
    void static loop();
    /**
     * @brief Deklarace statické funkce pro změnu stavu displeje
     * 
     * @param state Nový stav displeje 
     */
    void static change_display_state(fridge_display_state state);
    //Deklarace statické funkce pro nastavení displeje z nastavení uložené v EEPROM
    void static load_display_state_from_eeprom();
    //Deklarace statické funkce pro vrácení, zda je displej dostupný
    int static get_is_enable() {return FridgeDisplay::is_dislay_enable;}
    //Deklarace statické funkce pro vrácení aktuálního stavu displeje
    fridge_display_state static get_display_state() {return FridgeDisplay::display_state;}
    //Deklarace statické funkce pro povolení displeje
    void static enable_display();
    //Deklarace statické funkce pro zakázání displeje
    void static disable_display();
};

//Deklarace třídy pro čtení/zápis dat displeje chytré ledničky (stav vypnuto/zapnuto)
class DisplayEnableCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};
//Deklarace třídy pro čtení/zápis dat displeje chytré ledničky (nastavení zobrazení) 
class DisplayStateCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};