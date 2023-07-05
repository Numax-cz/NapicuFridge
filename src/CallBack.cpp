#include <include/CallBack.h>

// při připojení zařízení nastav proměnnou na log1
void ServerCallBack::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    esp_bd_addr_t data_from_eeprom;
    //Přečtení hodnoty MAC adresy z EEPROM
    for (int i = 0; i < EEPROM_SIZE; ++i) {
        data_from_eeprom[i] = EEPROM.read(MAC_ADDRESS_EEPROM_ADDR + i);
        //Vypsání hodnoty do konzole
        Serial.println(data_from_eeprom[i]);
    }

    //Pokud je uložená MAC adresa proveď následující
    if(data_from_eeprom[0] != 0xFF) {
        //Vypsání hodnoty do konzole
        Serial.println("MAC adresa je uložená v EEPROM.");
        Serial.println(BLEAddress(data_from_eeprom).toString().c_str());  

        //Kontrola, zda se adresa připojovaného zařízení shoduje s uloženou adresou
        if(BLEAddress(param->connect.remote_bda).toString() != BLEAddress(data_from_eeprom).toString()) {
            //Odpojení zařízení v neshody
            esp_ble_gap_disconnect(param->connect.remote_bda);
            return;
        } 
    }

    digitalWrite(CONNECTION_LED, HIGH);
}

// při odpojení zařízení nastav proměnnou na log0
void ServerCallBack::onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    devicePaired = false;
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