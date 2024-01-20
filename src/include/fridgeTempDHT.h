#pragma once
#include "main.h"



class FridgeTempDHT {
private:
    // Proměnná pro DHT senzor 
    DHT *dht;
    // Proměnná BLE komunikačního kanálu pro odesílání dat
    BLECharacteristic *pCharacteristic;
    //Reference proměnné pro ukládání teploty 
    String& value;
    
public:
    /**
     * @brief Definice konstruktor pro vytvoření nové třídy DHT senzoru
     * 
     * @param pin Datový pin DHT senzoru 
     * @param uuid UUID pro komunikaci 
     * @param pService BLE služba
     * @param value Reference hodnoty pro teplotu
     */
    FridgeTempDHT(int pin, const char* uuid, BLEService* pService, String& value);
    //Definice dekonstruktoru
    ~FridgeTempDHT();
    //Definice funkce pro inicializaci DHT senzoru 
    void begin();
    //Definice funkce, která pošle data skrze BLE do připojeného zařízení
    void sendTemperature();
    //Definice funkce pro aktualizování teploty
    void updateTemperature();
};

