#pragma once
// Připojení potřebných knihoven
// Připojení potřebných knihoven
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <EEPROM.h>
#include <DHT.h>

#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include <include/serverCallBack.h>
#include <include/fridgeTempDHT.h>
#include <include/fridgeDisplay.h>

//Definice propojovacích pinů
//pro analogový vstup a LED diodu
#define CONNECTION_LED 19
#define TEST_LED 18
//Definice propojovacích pinů veškerých teploměrů 
#define DHT_TYPE DHT11  
#define DHT_INSIDE 17 
#define DHT_OUTSIDE 4

//Definice EEPROM pro ukládání MAC adresy 
#define MAC_EEPROM_SIZE 6 //Délka MAC adresy v bajtech
#define MAC_ADDRESS_EEPROM_ADDR 0 //Adresa v paměti EEPROM, na kterou bude MAC adresa uložena
//Definice EEPROM pro ukládání stavu displeje log0/log1
#define DISPLAY_AVAILABLE_EEPROM_SIZE 1 //Délka stavu displeje  -128 až 127
#define DISPLAY_AVAILABLE_ADRESS_EEPROM_ADDR MAC_EEPROM_SIZE //Adresa v paměti EEPROM, na kterou bude stav displeje uložen log0/log1


//Definice unikátních ID pro různé služby,
//pro vlastní UUID využijte generátor
//https://www.uuidgenerator.net/
#define SERVICE_UUID           "cea986c2-4405-11ee-be56-0242ac120002" 
#define CHARACTERISTIC_DISPLAY_ENABLE_UUID "cea98ac8-4405-11ee-be56-0242ac120002"
#define CHARACTERISTIC_DHT_INSIDE_UUID "cea98c12-4405-11ee-be56-0242ac120002"
#define CHARACTERISTIC_DHT_OUTSIDE_UUID "cea99162-4405-11ee-be56-0242ac120002"
#define CHARACTERISTIC_DISPLAY_STATE_UUID "52a25b48-4596-11ee-be56-0242ac120002"



//Definice, která určuje výchozí dostupnost displeje
#define DISPLAY_DEFAULT_AVAILABLE 1 //V tomto případě je displej při prvním zapnutí zapnutý

//Proměnná pro kontrolu připojených zařízení
extern bool devicePaired;



//Definice struktury pro nastavení chytré ledničky 
struct fridge_data
{
    //Proměnná pro ukládání mac adresy spárovaného zařízení
    BLEAddress* paired_device_address = nullptr;

    //Proměnná pro ukládání vnitřní teploty ledničky ve formátu string 
    String in_temp = "";

    //Proměnná pro ukládání venkovní teploty ve formátu string 
    String out_temp = "";
};

//Proměnná pro globální nastavení chytré ledničky 
extern fridge_data FridgeData;


//Funkce, která vrátí mac adresu spárované zařízení z EEPROM
BLEAddress* read_paired_device_mac_address_from_eeprom();

//Funkce, která uvede zařízení do továrního nastavení
void factory_reset();