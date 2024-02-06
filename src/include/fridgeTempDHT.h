#pragma once
#include "main.h"

class FridgeTempDHT {
private:
    //Deklarace proměnné pro DHT senzor 
    DHT *dht;
    //Deklarace BLE komunikačního kanálu pro odesílání dat
    BLECharacteristic *pCharacteristic;
    //Deklarace referenční proměnné pro ukládání teploty 
    String& value; 
public:
    /**
     * @brief Deklarace konstruktor třídy
     * 
     * @param pin Datový pin DHT senzoru 
     * @param uuid UUID pro komunikaci 
     * @param pService BLE služba
     * @param value Reference hodnoty pro teplotu
     */
    FridgeTempDHT(int pin, const char* uuid, BLEService* pService, String& value);
    //Deklarace dekonstruktoru třídy
    ~FridgeTempDHT();
    //Deklarace funkce pro inicializaci DHT senzoru 
    void begin();
    //Deklarace funkce, která pošle data skrze BLE do připojeného zařízení
    void sendTemperature();
    //Deklarace funkce pro aktualizování teploty
    void updateTemperature();
};

