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
    FridgeTempDHT(int pin, const char* uuid, BLEService* pService, String& value);
    ~FridgeTempDHT();
    void begin();
    void sendTemperature();
    void updateTemperature();

};

