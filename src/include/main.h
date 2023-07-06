#pragma once
// Připojení potřebných knihoven
// Připojení potřebných knihoven
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <EEPROM.h>
#include <DHT.h>
#include <include/CallBack.h>
#include <include/fridgeTempDHT.h>


//Definice propojovacích pinů
//pro analogový vstup a LED diodu
#define readPin 32
#define CONNECTION_LED 19
#define TEST_LED 18

//Definice propojovacích pinů veškerých teploměrů 
#define DHT_TYPE DHT11  
#define DHT_INSIDE 17 

//Definice EEPROM pro ukládání MAC adresy 
#define EEPROM_SIZE 6 //Délka MAC adresy v bajtech
#define MAC_ADDRESS_EEPROM_ADDR 0 //Adresa v paměti EEPROM, na kterou bude MAC adresa uložena

//Definice unikátních ID pro různé služby,
//pro vlastní UUID využijte generátor
//https://www.uuidgenerator.net/
#define SERVICE_UUID           "5ff8dace-0914-11ee-be56-0242ac120002" 
#define CHARACTERISTIC_UUID_RX "5ff8dda8-0914-11ee-be56-0242ac120002"
#define CHARACTERISTIC_DHT_INSIDE_TX "5ff8e046-0914-11ee-be56-0242ac120002"


//Proměnná pro kontrolu připojených zařízení
extern bool devicePaired;


//Definice struktury pro nastavení chytré ledničky 
struct fridge_data
{
    //Proměnná pro ukládání mac adresy spárovaného zařízení
    BLEAddress* paired_device_address = nullptr;
};

//Proměnná pro globální nastavení chytré ledničky 
extern fridge_data FridgeData;


//Funkce, která vrátí mac adresu spárované zařízení z EEPROM
BLEAddress* read_paired_device_mac_address_from_eeprom();