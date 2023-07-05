
#pragma once

// připojení potřebných knihoven
#include "main.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <EEPROM.h>


// třída pro kontrolu připojení
class ServerCallBack : public BLEServerCallbacks {
public:
    void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param);
    void onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param);
};

// třída pro příjem zprávy
class CharacteristicCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pCharacteristic);
};