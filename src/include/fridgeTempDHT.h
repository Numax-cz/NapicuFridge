#pragma once
#include "main.h"
#include <DHT.h>
#include <BLEDevice.h>
#include <BLE2902.h>


class FridgeTempDHT{
private:
    // Proměnná pro DHT senzor 
    DHT *dht;
    // Proměnná BLE komunikačního kanálu pro odesílání dat
    BLECharacteristic *pCharacteristic;
    
public:
    FridgeTempDHT(int pin, const char* uuid, BLEService* pService);
    ~FridgeTempDHT();
    void begin();
    void sendTemperature();

};

