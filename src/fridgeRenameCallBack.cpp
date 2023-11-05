#include <include/fridgeRenameCallBack.h>

void FridgeRenameCallBack::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string msg = pCharacteristic->getValue();

    //Spuštění funkce pro přejmenování zařízení a uložení vrácené hodnoty do proměnné
    esp_err_t errRc = esp_ble_gap_set_device_name(msg.c_str());

    //Pokud proměnná není rovna definici ESP_OK (0)
    if (errRc != ESP_OK) {
        //Vypsání chybné hlášky do konzole
        Serial.println("Nelze nastavit jméno zařízení");
    } else {
        //Vypsání hodnoty do konzole
        Serial.println("Zařízení bylo úspěšně přejmenováno");
    }
}