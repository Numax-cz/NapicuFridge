#pragma once
#include "main.h"

#define DISPLAY_W 128
#define DISPLAY_H 32

typedef enum {


    FRIDGE_DISPLAY_PAIR_TEXT = 0,

    FRIDGE_DISPLAY_IN_TEMP_1,
    FRIDGE_DISPLAY_OUT_TEMP_1,

} fridge_display_state;

class FridgeDisplay {
private:
    static inline Adafruit_SSD1306* display = NULL;

    static inline fridge_display_state display_state = FRIDGE_DISPLAY_PAIR_TEXT; 

    static inline bool is_dislay_enable = true;

    void static print_centered_text(String text, uint8_t font_size = 1, int16_t x = -1);


   static inline char message[]="Sparujte zarizeni";
   static inline int x, minX;

    void static print_pair_text();

public:
    void static begin();
    void static loop();
    void static change_display_state(fridge_display_state state) {FridgeDisplay::display_state = state;}
    int static get_is_enable() {return FridgeDisplay::is_dislay_enable;}
    fridge_display_state static get_display_state() {return FridgeDisplay::display_state;}
    void static disable_display();
    void static enable_display();


};



//Třída pro správu displeje chytré ledničky
class DisplayEnableCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};

class DisplayStateCharacteristicCallback : public BLECharacteristicCallbacks {
public: 
    void onWrite(BLECharacteristic *pCharacteristic);
    void onRead(BLECharacteristic *pCharacteristic);
};