#include <include/dataJSONManager.h>
#include <SPIFFS.h>

void DataJSONReadySendCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {
    //Proměnná pro ukládání přijaté zprávy
    std::string prijataZprava = pCharacteristic->getValue();

    //Pokud obsahuje znak 1 provede se následující
    if (prijataZprava == "1") {
        //Nastavení proměnné určující zda je zařízení připraveno k odeslání dat k připojenému zařízení na log1
        DataJSONManager::set_ready_to_send();
    }
}

/**
 * @brief Statická funkce pro inicializaci DataJsonManager 
 * 
 * @param pService BLE služba
 * @param notify_uuid UUID pro oznamování naměřených dat
 * @param ready_to_send_uuid UUID pro vynucení odeslání dat
 */
void DataJSONManager::begin(BLEService* pService, const char* notify_uuid, const char* ready_to_send_uuid) {
    //Funkce pro inicializaci SPIFFS, pokud došlo k problému provede se následující
    if(!SPIFFS.begin(true, "/spiffs", 5)){
        //Vypsání hodnoty do konzole
        Serial.println("Neúspěšná inicializace systému SPIFFS");
        return;
    }

    //Vypsání hodnoty do konzole
    Serial.println("Inicializace systému SPIFFS proběhla úspěšně!");

    //Vytvoření BLE komunikačního kanálu pro komunikaci
    DataJSONManager::pCharacteristic = pService->createCharacteristic(
        notify_uuid,
        BLECharacteristic::PROPERTY_INDICATE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_READ
    );
    
    //Přiřazení deskriptoru k této charakteristice.
    DataJSONManager::pCharacteristic->addDescriptor(new BLE2902());

    //Vytvoření BLE komunikačního kanálu pro komunikaci
    BLECharacteristic *readyDataCharacteristic = pService->createCharacteristic(
        ready_to_send_uuid,
        BLECharacteristic::PROPERTY_WRITE
    );

    //Přiřazení zpětného volání k této charakteristice.
    readyDataCharacteristic->setCallbacks(new DataJSONReadySendCharacteristicCallback());

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
    //Uložíme aktuální čas běhu do konstantní proměnné time 
    const unsigned long time = millis();
    //Načasování programu
    if(time >= DataJSONManager::time_now + DEFAULT_JSON_DATA_SAVE_INTERVAL) {
        if(PowerManager::is_power_on()) {
            //Přičte se statická proměnná delay k proměnné určující aktuální dobu
            DataJSONManager::time_now += DEFAULT_JSON_DATA_SAVE_INTERVAL;
            //Spuštění funkce pro zápist dat 
            DataJSONManager::write();
        }
    }

    //Pokud je proměnná určující zda je zařízení připraveno k odeslání dat k připojenému zařízení provede se následující 
    if(DataJSONManager::ready_to_send) {
        //Pokud je zařízení připojené provede se následující
        if(devicePaired) {
            //Spuštění funkce pro odeslání naměřených hodnot do připojeného zaířzení 
            DataJSONManager::send();
            //Nastavení proměnné určující zda je zařízení připraveno k odeslání dat k připojenému zařízení na log0
            DataJSONManager::ready_to_send = false;
        }
    }
}

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
    file.flush();

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

    //Vymazání paměti json dokumentu
    doc.clear();
    //Spuštění funkce pro zavření souboru
    file.flush();

    //Nastavení proměnné určující zda je zařízení připraveno k odeslání dat k připojenému zařízení na log1 
    DataJSONManager::ready_to_send = true;

    //Pokud je zapnutý vývojářký režim, provede se následující 
    if(DEV_MODE) {
        //Přečtení souboru a uložení třídy do proměnné
        File file2 = SPIFFS.open(DEFAULT_JSON_DATA_FILE_NAME, FILE_READ);
        //Vypíšeme obsah souboru do konzole
        Serial.println(file2.readString());
        //Spuštění funkce pro zavření souboru
        file2.flush();
    }
}


//Funkce, která odešle naměřená data do připojeného zařízení
void DataJSONManager::send() {
    /////////////////////////////////////////////////////////////////////
    //Jelikož skrze BLE lze poslat v jeden čas 20 bajtů pro charakteristiku musíme
    //v následujícím bloku kódu rozkouskovat data po 20 bajtech

    DynamicJsonDocument doc(2048);
    //Přečtení souboru a uložení třídy do proměnné
    File file_data = SPIFFS.open(DEFAULT_JSON_DATA_FILE_NAME, FILE_READ);
    //Vypíšeme obsah souboru do konzole

    DeserializationError error = deserializeJson(doc, file_data.readString());
    //Spuštění funkce pro zavření souboru
    file_data.flush();

    //Vypsání hodnoty do konzole
    Serial.println("Odeslání informací o naměřených hodnot");
    //Pošleme výchozí indikaci jako začátek bloku dat
    DataJSONManager::pCharacteristic->setValue("#START");
    //Odeslání zprávy skrze BLE do připojeného zařízení
    DataJSONManager::pCharacteristic->notify();

    //Proměnná pro uložení hodnoty JSONu
    String jsonString = "";
    //Vytvoříme a uložíme JSON dokument do proměnné
    serializeJson(doc, jsonString);

    //Uložíme velikost stringu do proměnné 
    int data_length = jsonString.length();
    //Deklarace proměnné pro ukládání aktuální velikost poslaných dat
    int current_index = 0;

    //Dokud je aktuální index menší než velikost dat proveď následující
    while (current_index < data_length) {
        //Uložíme velikost zbývajících
        int remaining_bytes = data_length - current_index;
        //Vypočítáme velikost aktuálního balíčku a uložíme jej do proměnné
        int packet_size = min(remaining_bytes, 20); 
        //Data balíčku ve formátu string 
        String packet_data = jsonString.substring(current_index, current_index + packet_size);
        //Nastavení hodnotu balíčku pro charakteristiku 
        DataJSONManager::pCharacteristic->setValue(packet_data.c_str());
        //Odeslání balíčku skrze BLE do připojeného zařízení
        DataJSONManager::pCharacteristic->notify();
        //Přičteme k aktuální velikost poslanou velikost aktuálního balíčku
        current_index += packet_size;  
    } 

    //Pošleme výchozí indikaci jako konec bloku dat
    DataJSONManager::pCharacteristic->setValue("#END");
    //Odeslání zprávy skrze BLE do připojeného zařízení
    DataJSONManager::pCharacteristic->notify();
    //Vymazání paměti json dokumentu
    doc.clear();
    /////////////////////////////////////////////////////////////////////
}