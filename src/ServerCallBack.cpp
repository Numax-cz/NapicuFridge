#include <include/ServerCallBack.h>
// při připojení zařízení nastav proměnnou na log1
void ServerCallBack::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    if(FridgeData.paired_device_address != nullptr) {
        if(FridgeData.paired_device_address->toString() != BLEAddress(param->connect.remote_bda).toString()) {
            esp_ble_gap_disconnect(param->connect.remote_bda);
            return;
        }
    } 


    digitalWrite(CONNECTION_LED, HIGH);
}

// při odpojení zařízení nastav proměnnou na log0
void ServerCallBack::onDisconnect(BLEServer* pServer) {
    devicePaired = false;
    Serial.println("Disconected");
    digitalWrite(CONNECTION_LED, LOW);
    BLEDevice::startAdvertising();
}


void CharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    // při příjmu zprávy proveď následující

    
    // proměnná pro ukládání přijaté zprávy
    uint8_t prijataZprava = *pCharacteristic->getData();

    Serial.print("Prijata zprava: ");
    Serial.print(prijataZprava);
    Serial.println();

    //kontrola přijaté zprávy
    //pokud obsahuje znak 1, rozsviť LED diodu
    if (prijataZprava == 1) {
        Serial.println("Zapnutí LED!");
        digitalWrite(TEST_LED, HIGH);
    }
    // pokud obsahuje znak 0, zhasni LED diodu
    else if (prijataZprava == 0) {
        Serial.println("Vypnutí LED!");
        digitalWrite(TEST_LED, LOW);
    }
};


