
#pragma once

// připojení potřebných knihoven
#include "main.h"

//Deklarace třídy pro kontrolu připojení
class ServerCallBack : public BLEServerCallbacks {
public:
    void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param);
    void onDisconnect(BLEServer* pServer);
};

//Deklarace třídy pro příjem zprávy
class CharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic);
};