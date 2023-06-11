#include <include/CallBack.h>

// při připojení zařízení nastav proměnnou na log1
void ServerCallBack::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    zarizeniPripojeno = true;
    digitalWrite(CONNECTION_LED, HIGH);
}

// při odpojení zařízení nastav proměnnou na log0
void ServerCallBack::onDisconnect(BLEServer* pServer) {
    zarizeniPripojeno = false;
    digitalWrite(CONNECTION_LED, LOW);
    //BLEDevice::startAdvertising();
}


void CharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    // při příjmu zprávy proveď následující

    // načti přijatou zprávu do proměnné
    prijataZprava = pCharacteristic->getValue();
    // pokud není zpráva prázdná, vypiš její obsah
    // po znacích po sériové lince
    if (prijataZprava.length() > 0) {
        Serial.print("Prijata zprava: ");
        for (int i = 0; i < prijataZprava.length(); i++) {
            Serial.print(prijataZprava[i]);
        }
        Serial.println();
        //kontrola přijaté zprávy
        //pokud obsahuje znak A, rozsviť LED diodu
        if (prijataZprava.find("A") != -1) {
        Serial.println("Zapnutí LED!");
        digitalWrite(TEST_LED, HIGH);
        }
        // pokud obsahuje znak B, zhasni LED diodu
        else if (prijataZprava.find("B") != -1) {
        Serial.println("Vypnutí LED!");
        digitalWrite(TEST_LED, LOW);
        }
    }
};