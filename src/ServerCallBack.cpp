#include <include/serverCallBack.h>
// při připojení zařízení nastav proměnnou na log1
void ServerCallBack::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    //Pokud je uložena adresa spárovaného zařízení provede se následující 
    if(FridgeData.paired_device_address != nullptr) {
        //Pokud je uložená adresa spárovaného zařízení není rovna aktuální připojující se adrese provede se následující 
        if(FridgeData.paired_device_address->toString() != BLEAddress(param->connect.remote_bda).toString()) {
            //Spuštění funkce pro odpojení připojeného zařízení
            esp_ble_gap_disconnect(param->connect.remote_bda);
            return;
        }
    } 
}

// při odpojení zařízení nastav proměnnou na log0
void ServerCallBack::onDisconnect(BLEServer* pServer) {
    //Nastavení proměnné ukládádající zda je zařízení připojené na log0
    devicePaired = false;
    //Pokud je zapnutý vývojářký režim, provede se následující 
    if(DEV_MODE) {
        //Vypsání hodnoty do konzole
        Serial.println("Disconected");
    }
    //Zapnutí viditelnosti BLE
    BLEDevice::startAdvertising();
}