#include <include/fridgeDisplay.h>


void FridgeDisplay::begin() {
    FridgeDisplay::display = new Adafruit_SSD1306(128, 32, &Wire, -1);
    FridgeDisplay::display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void FridgeDisplay::loop() {
    FridgeDisplay::display->clearDisplay();
    FridgeDisplay::display->setTextSize(1);
    FridgeDisplay::display->setTextColor(WHITE);
    FridgeDisplay::display->setCursor(0,0);
    FridgeDisplay::display->println("Aaahoj");
    FridgeDisplay::display->display();
}