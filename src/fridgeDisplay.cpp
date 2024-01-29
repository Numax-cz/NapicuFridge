#include <include/fridgeDisplay.h>
#include <Fonts/FreeMonoBold9pt7b.h>

//Funkce pro inicializaci displeje
void FridgeDisplay::begin() {
    //Vytvoření nové třídy displeje
    FridgeDisplay::display = new Adafruit_SSD1306(DISPLAY_W, DISPLAY_H, &Wire, -1);
    //Spuštění funkce pro inicializaci displeje
    FridgeDisplay::display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    //Získání on/off stavu z EEPROM 
    uint8_t on_off_state_data = EEPROM.read(DISPLAY_AVAILABLE_ADRESS_EEPROM_ADDR);

    //Pokud není uložena hodnota v EEPROM provede se následující 
    if(on_off_state_data == 0xFF) {
        //Nastavení výchozí hodnoty
        on_off_state_data = DISPLAY_DEFAULT_AVAILABLE;
    } 

    //Pokud jsou uložená data rovny log1 provede se následující
    if(on_off_state_data == 1) {
        //Povolení displeje
        FridgeDisplay::enable_display();
    } else {
        //Zakázání displeje
        FridgeDisplay::disable_display();
    }

    //Nastavení pozice
    FridgeDisplay::x = FridgeDisplay::display->width();
    //Nastavení min pozice
    FridgeDisplay::minX = -12 * strlen(message);
}

//Funkce pro nastavení displeje z nastavení uložené v EEPROM
void FridgeDisplay::load_display_state_from_eeprom() {
    //Pokud je aktuální nastavení displeje nastaveno na zobrazování párovacího textu provede se následující
    if(FridgeDisplay::get_display_state() == FRIDGE_DISPLAY_PAIR_TEXT) {
        //Získání on/off stavu z EEPROM 
        uint8_t settings_data = EEPROM.read(DISPLAY_MODE_ADRESS_EEPROM_ADDR);

        //Pokud není uložena hodnota v EEPROM provede se následující 
        if(settings_data == 0xFF) {
            //Nastavení výchozí hodnoty
            settings_data = DEFAULT_DISPLAY_MODE;
        }

        //Statické castování uint8_t na fridge_display_state a následené nastavení stavu displeje
        FridgeDisplay::change_display_state(static_cast<fridge_display_state>(settings_data));
    }
}

//Funkce pro povolení displeje
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

//Funkce pro zakázání displeje
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

void FridgeDisplay::change_display_state(fridge_display_state state) {
    //Zapsání nastavení displeje do EEPROM
    EEPROM.write(DISPLAY_MODE_ADRESS_EEPROM_ADDR, state);
    //Nastavení statické proměnné
    FridgeDisplay::display_state = state;
    //Potvrzení změn
    EEPROM.commit();
}

//Funkce pro aktualizaci 
void FridgeDisplay::loop() {
    if(FridgeDisplay::display_state == FRIDGE_DISPLAY_PAIR_TEXT) {
        //Spuštění funkce pro vypsání textu pro párování
        FridgeDisplay::print_pair_text();
    }
    else if (FridgeDisplay::display_state == FRIDGE_DISPLAY_IN_TEMP_1) {
        //Uložení vnitřní teploty do proměnné
        String temp = FridgeData.in_temp;
        //Přidání k proměnné znak stupně 
        temp += (char)247;
        //Přidání znaku
        temp += "C";
        FridgeDisplay::print_centered_text(temp, 2);
    }
    else if (FridgeDisplay::display_state == FRIDGE_DISPLAY_OUT_TEMP_1) {
        //Uložení venkovní teploty do proměnné
        String temp = FridgeData.out_temp;
        //Přidání k proměnné znak stupně 
        temp += (char)247;
        //Přidání znaku
        temp += "C";
        FridgeDisplay::print_centered_text(temp, 2);
    }
    else if (FridgeDisplay::display_state == FRIDGE_DISPLAY_COOLER_TEMP) {
        //Uložení teploty chladiče do proměnné
        String temp = FridgeData.cooler_temp;
        //Přidání k proměnné znak stupně 
        temp += (char)247;
        //Přidání znaku
        temp += "C";
        FridgeDisplay::print_centered_text(temp, 2);
    }
}

/**
 * @brief Funkce, která vypíše text vycentrovaně
 * 
 * @param text Text, který se má zobrazit 
 * @param font_size Velikost textu
 * @param x Pozice x textu
 */
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

//Funkce pro vypsání textu pro párování 
void FridgeDisplay::print_pair_text() {
    //Spuštění funkce pro výpis vycentrovaného textu
    FridgeDisplay::print_centered_text(message, 2, x);
    //Změna pozice, číslo vyjadřuje rychlost posunu v px  
    x=x-1;
    if(x < minX) x= FridgeDisplay::display->width();
}