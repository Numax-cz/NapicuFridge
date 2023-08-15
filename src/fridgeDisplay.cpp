#include <include/fridgeDisplay.h>
#include <Fonts/FreeMonoBold9pt7b.h>



void FridgeDisplay::begin() {
    FridgeDisplay::display = new Adafruit_SSD1306(DISPLAY_W, DISPLAY_H, &Wire, -1);
    FridgeDisplay::display->begin(SSD1306_SWITCHCAPVCC, 0x3C);

    uint8_t data = EEPROM.read(DISPLAY_AVAILABLE_ADRESS_EEPROM_ADDR);

    if(data == 0xFF) {
        data = DISPLAY_DEFAULT_AVAILABLE;
    } 

    if(data == 1) {
        FridgeDisplay::enable_display();
    } else {
        FridgeDisplay::disable_display();
    }




    FridgeDisplay::display->setTextSize(2);
    FridgeDisplay::display->setTextColor(WHITE);
    FridgeDisplay::display->setTextWrap(false);
    FridgeDisplay::x = FridgeDisplay::display->width();
    FridgeDisplay::minX = -12 * strlen(message);

}


void FridgeDisplay::enable_display() {
    //Zapsání log1 hodnoty do EEPROM
    EEPROM.write(DISPLAY_AVAILABLE_ADRESS_EEPROM_ADDR, 1);
    //Nastavení proměnné na log1
    FridgeDisplay::is_dislay_enable = true;
    //Zapnutí displeje
    FridgeDisplay::display->ssd1306_command(SSD1306_DISPLAYON);
    //Potvrzení změn
    EEPROM.commit();
}

void FridgeDisplay::disable_display() {
    //Zapsání log0 hodnoty do EEPROM
    EEPROM.write(DISPLAY_AVAILABLE_ADRESS_EEPROM_ADDR, 0);
    //Nastavení proměnné na log0
    FridgeDisplay::is_dislay_enable = false;
    //Vyčištění displeje
    FridgeDisplay::display->clearDisplay();
    //Vypnutí displeje
    FridgeDisplay::display->ssd1306_command(SSD1306_DISPLAYOFF);
    //Potvrzení změn
    EEPROM.commit();
}

void FridgeDisplay::loop() {






    if(FridgeDisplay::display_state == FRIDGE_DISPLAY_PAIR_TEXT) {
        FridgeDisplay::print_pair_text();
    }
    else if (FridgeDisplay::display_state == FRIDGE_DISPLAY_IN_TEMP_1) {
        String temp = FridgeData.in_temp;
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