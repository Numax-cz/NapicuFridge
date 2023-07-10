#pragma once
#include "main.h"


class FridgeDisplay {
private:
    static inline Adafruit_SSD1306* display = NULL;

public:
    void static begin();
    void static loop();

};


