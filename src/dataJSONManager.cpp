#include <include/dataJSONManager.h>
#include <SD.h>


//void DataJSONCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
//     //Vypsání hodnoty do konzole
//     Serial.println("Odeslání dat o zaznamenaných informací");
//     //Přečtení souboru a uložení třídy do proměnné
//     File file = SPIFFS.open(DEFAULT_JSON_DATA_FILE_NAME, FILE_READ);
//     //Nastavení hodnoty charakteristiky 
//     pCharacteristic->setValue(file.readString().c_str());
//     //Odeslání zprávy skrze BLE do připojeného zařízení
//     pCharacteristic->notify();
//     //Spuštění funkce pro zavření souboru
//     file.flush();
// }

//Funkce pro inicializaci DataJsonManager
void DataJSONManager::begin(BLEService* pService) {
    //Vytvoření BLE komunikačního kanálu pro komunikaci
    DataJSONManager::pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_JSON_DATA_UUID,
        BLECharacteristic::PROPERTY_INDICATE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_READ
    );
    
    //Přiřazení deskriptoru k této charakteristice.
    DataJSONManager::pCharacteristic->addDescriptor(new BLE2902());      
    

    //Funkce pro inicializaci SPIFFS, pokud došlo k problému provede se následující
    if(!SPIFFS.begin(false, "/spiffs", 5)){
        //Vypsání hodnoty do konzole
        Serial.println("Neúspěšná inicializace systému SPIFFS");
        return;
    }

    //Vypsání hodnoty do konzole
    Serial.println("Inicializace systému SPIFFS proběhla úspěšně!");

    //Přečtení souboru a uložení třídy do proměnné
    File file = SPIFFS.open(DEFAULT_JSON_DATA_FILE_NAME, FILE_WRITE);
    //Pokud došlo k problému při otevírání souboru, provede se následující:
    if(!file){
        //Vypsání hodnoty do konzole
        Serial.println("Došlo k chybě při otevírání souboru pro zápis");
        return;
    }

    //Spuštění funkce pro zavření souboru
    file.flush(); 

}

//Loop funkce pro DataJSONManager
void DataJSONManager::loop() {
    DataJSONManager::write();
}



//TODO 
//TODO 
//TODO 
//TODO 
//Funkce pro zápis dat do souboru
void DataJSONManager::write() {
    DynamicJsonDocument doc(2048);
    //Přečtení souboru a uložení třídy do proměnné
    File file = SPIFFS.open(DEFAULT_JSON_DATA_FILE_NAME, FILE_READ);
    
    char fileBuffer[file.size()+1];
    file.readBytes(fileBuffer, file.size());
    fileBuffer[file.size()]='\0';
    DeserializationError error = deserializeJson(doc, fileBuffer, file.size()+1);

    //Spuštění funkce pro zavření souboru
    file.close();

    //Přečtení souboru a uložení třídy do proměnné
    file = SPIFFS.open(DEFAULT_JSON_DATA_FILE_NAME, FILE_WRITE);

    //Pokud Array neexistuje provede se následující
    if (!doc.containsKey(DEFAULT_JSON_IN_TEMP_NAME)) {
        //Vytvoří se nový prázdný array
        doc.createNestedArray(DEFAULT_JSON_IN_TEMP_NAME);
    }

    //Pokud Array neexistuje provede se následující
    if (!doc.containsKey(DEFAULT_JSON_OUT_TEMP_NAME)) {
        //Vytvoří se nový prázdný array
        doc.createNestedArray(DEFAULT_JSON_OUT_TEMP_NAME);
    }

    //Pokud Array neexistuje provede se následující
    if (!doc.containsKey(DEFAULT_JSON_COOLER_TEMP_NAME)) {
        //Vytvoří se nový prázdný array
        doc.createNestedArray(DEFAULT_JSON_COOLER_TEMP_NAME);
    }

    //Nastaví hodnotu na prázdné pole a následné uložení do proměnné
    JsonArray in_temp_data = doc[DEFAULT_JSON_IN_TEMP_NAME].as<JsonArray>();
    //Přidání hodnoty do array 
    in_temp_data.add(FridgeData.in_temp);
    
    //Nastaví hodnotu na prázdné pole a následné uložení do proměnné
    JsonArray out_temp_data = doc[DEFAULT_JSON_OUT_TEMP_NAME].as<JsonArray>();
    //Přidání hodnoty do array 
    out_temp_data.add(FridgeData.out_temp);

    //Nastaví hodnotu na prázdné pole a následné uložení do proměnné
    JsonArray cooler_temp_data = doc[DEFAULT_JSON_COOLER_TEMP_NAME].as<JsonArray>();
    //Přidání hodnoty do array 
    cooler_temp_data.add(FridgeData.cooler_temp);

    //Uložení json dokumentu do souboru, pokud dojde k chybě provede se následující
    if (serializeJson(doc, file) == 0) {
        //Vypsání hodnoty do konzole
        Serial.println("Chyba při zápisu dat");
    }   

    String jsonString = "";
    serializeJson(doc, jsonString);

    //Nastavení hodnoty charakteristiky 
    DataJSONManager::pCharacteristic->setValue(jsonString.c_str());
    //Odeslání zprávy skrze BLE do připojeného zařízení
    DataJSONManager::pCharacteristic->notify();

    //Vymazání paměti json dokumentu
    doc.clear();
    //Spuštění funkce pro zavření souboru
    file.close(); //TODO FLUSH
       
    //Pokud je zapnutý vývojářký režim, provede se následující 
    if(DEV_MODE) {
        //Přečtení souboru a uložení třídy do proměnné
        File file2 = SPIFFS.open(DEFAULT_JSON_DATA_FILE_NAME, FILE_READ);
        //Vypíšeme obsah souboru do konzole
        Serial.println(file2.readString());
        //Spuštění funkce pro zavření souboru
        file2.close();
    }
}