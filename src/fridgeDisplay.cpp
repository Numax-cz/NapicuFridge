#include <include/fridgeDisplay.h>
#include <Fonts/FreeMonoBold9pt7b.h>



void FridgeDisplay::begin() {
    FridgeDisplay::display = new Adafruit_SSD1306(DISPLAY_W, DISPLAY_H, &Wire, -1);
    FridgeDisplay::display->begin(SSD1306_SWITCHCAPVCC, 0x3C);




    FridgeDisplay::display->setTextSize(2);
    FridgeDisplay::display->setTextColor(WHITE);
    FridgeDisplay::display->setTextWrap(false);
    FridgeDisplay::x = FridgeDisplay::display->width();
    FridgeDisplay::minX = -12 * strlen(message);

}

void FridgeDisplay::loop() {






    if(FridgeDisplay::display_state == FRIDGE_DISPLAY_PAIR_TEXT) {
        FridgeDisplay::print_pair_text();
    }
    else if (FridgeDisplay::display_state == FRIDGE_DISPLAY_IN_TEMP_1) {
        String temp = "69.5";
        temp += (char)247;
        temp += "C";
        FridgeDisplay::print_centered_text(temp, 2);

    }



}

void FridgeDisplay::print_centered_text(String text, uint8_t font_size, int16_t x) {
    //Nastavení veškerých pixelů na log0
    FridgeDisplay::display->clearDisplay();
    //Nastavení velikosti textu
    FridgeDisplay::display->setTextSize(font_size);
    //Nastavení barvy textu (při našem použitém oledu se zobrazí pouze modrý text)
    FridgeDisplay::display->setTextColor(WHITE);
    //Zakázání formátování textu
    FridgeDisplay::display->setTextWrap(false);
    //Vypočítání středu osy X v závislosti na textu a velikosti textu
    if(x == -1) x = (DISPLAY_W - (text.length() * (font_size * 6))) / 2;
    //Vypočítání středu osy Y v závislosti na textu a velikosti textu
    int16_t y = (DISPLAY_H / 2) - (font_size * 8) / 2;
    //Nastavení curzoru podle proměnné x a y
    FridgeDisplay::display->setCursor(x, y);
    //Vypsání hodnoty
    FridgeDisplay::display->println(text);
    //Zobrazení hodnoty na display
    FridgeDisplay::display->display();
}



void FridgeDisplay::print_pair_text() {

    FridgeDisplay::print_centered_text(message, 2, x);

   //Změna pozice, číslo vyjadřuje rychlost posunu v px  
   x=x-1; 
   if(x < minX) x= FridgeDisplay::display->width();
}