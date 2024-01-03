#pragma once
// Připojení potřebných knihoven
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <EEPROM.h>
#include <DHT.h>
#include <DigiPotX9Cxxx.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Thermistor.h>
#include <NTC_Thermistor.h>
#include <sstream>

//Připojení potřebných knihoven z include
#include <include/serverCallBack.h>
#include <include/fridgeTempDHT.h>
#include <include/fridgeDisplay.h>
#include <include/relayModule.h>
#include <include/buttonManager.h>
#include <include/fanController.h>
#include <include/powerManager.h>
#include <include/piezoManager.h>
#include <include/errorChecker.h>
#include <include/thermistorManager.h>
#include <include/dataJSONManager.h>
#include <include/uptime.h>
#include <include/fridgeFactoryReset.h>
#include <include/digitalPotentiometer.h>
#include <include/rgbManager.h>
/////////////////////////////////////////////////////////////////////

#define DEV_MODE 1

//Definice výchozího názvu BLE názvu
#define DEFAULT_BLE_NAME "NapicuFridge"

//Definice propojovacích pinů

//pro analogový vstup a LED diodu
#define CONNECTION_LED 19
//Definice pinu pro PIEZO
#define PIEZO_PIN 18
//Definice pinu pro snímání dveří 
#define DOOR_PIN 13
//Definice propojovacích pinů veškerých teploměrů 
#define DHT_TYPE DHT11 //Typ DHT senzoru
#define DHT_INSIDE 17  // Pin vnitřního DHT senzoru
#define DHT_OUTSIDE 16 //Pin venkovního DHT senzoru

//Definice pinu pro RGB osvětlení 
#define RGB_PIN 19
//Definice počtů led na RGB osvětlení
#define RGB_LED_COUNT 12

//Definice pinu tlačítka pro reset zařízení
#define RESET_BUTTON 5

//Definice pinů digitálního potenciometru
#define X9_CS  14 //Pin CS pinu potenciometru
#define X9_INC 27 //Pin INC pinu potenciometru
#define X9_UD  26 //Pin UD pinu potenciometru 

//Definice pinů termistoru
#define COOLER_NTC_SENSOR_PIN             4 //Pin pro příjem dat
#define COOLER_NTC_REFERENCE_RESISTANCE    9830 // Referenční hodnota rezistoru 
#define COOLER_NTC_NOMINAL_RESISTANCE     10000 // Nominální odpor
#define COOLER_NTC_NOMINAL_TEMPERATURE    25 //Nominální teplota 
#define COOLER_NTC_B_VALUE                3950 //beta hodnota 
#define COOLER_NTC_STM32_ANALOG_RESOLUTION 4095 //Analogové Rozlišení 

//Definicie pinů chladících ventilátorů
#define COOLING_FAN_PWM 15 //Pin pro řízení PWM  
#define COOLING_FAN_TACH 0 //Pin pro zaznamenávání otáček

//Definice pinů relátek
#define RELAY_PELTIER_PIN 32 //Pin relátka pro hlavní napájení peltierů   
#define RELAY_PELTIER_POWER_MODE_PIN 23 //Pin relátka pro řízení režimu napájení 
#define RELAY_PWM_FANS_MODULE_PIN 25 //Pin relátka pro napájení chladících ventilátorů 
#define RELAY_IN_FANS_MODULE_PIN 33 //Pin relátka vnitřních ventilátorů

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//Definice EEPROM adres 

//Definice EEPROM pro ukládání MAC adresy 
#define MAC_EEPROM_SIZE 6 //Délka MAC adresy v bajtech
#define MAC_ADDRESS_EEPROM_ADDR 0 //Adresa v paměti EEPROM, na kterou bude MAC adresa uložena
//Definice EEPROM pro ukládání stavu displeje log0/log1
#define DISPLAY_AVAILABLE_EEPROM_SIZE 1 //Délka stavu displeje  -128 až 127
#define DISPLAY_AVAILABLE_ADRESS_EEPROM_ADDR MAC_EEPROM_SIZE //Adresa v paměti EEPROM, na kterou bude stav displeje uložen log0/log1
//Definice EEPROM pro ukládání stavu napájení ledničky
#define POWER_MODE_EEPROM_SIZE 1 //Stav napájení -128 až 127
#define POWER_MODE_EEPROM_ADDR DISPLAY_AVAILABLE_ADRESS_EEPROM_ADDR + 1 //Adresa v paměti EEPROM, na kterou bude mód napájení uložen podle enumerace fridge_power_mode
//Definice EEPROM pro ukládání stavu vnitřních ventilátorů log0/log1
#define IN_FANS_EEPROM_SIZE 1
#define IN_FANS_EEPROM_ADDR POWER_MODE_EEPROM_ADDR + 1 //Adresa v paměti EEPROM na kterou bude stav vnitřních ventilátorů uložen log0/log1
//Definice EEPROM pro ukládání zda má piezo upozornit na chybu
#define PIEZO_ON_ERROR_SIZE 1 //Délka stavu piezo -128 až 127
#define PIEZO_ON_ERROR_ADDR IN_FANS_EEPROM_ADDR + 1 //Adresa v paměti EEPROM na kterou bude stav piezo ukládat
//Definice EEPROM pro ukládání zda se má lednička po otevření dveří pozastavit log0/log1
#define FRIDGE_PAUSE_ON_DOOR_OPEN_SIZE 1 //Délka stavu -128 až 127
#define FRIDGE_PAUSE_ON_DOOR_OPEN_ADDR PIEZO_ON_ERROR_ADDR + 1 //Adresa v paměti EEPROM na kterou se bude hodnota ukládat
//Definice EEPROM pro ukládání zda má být světlo povoleno při otevření dveří 
#define LED_AVAILABLE_EEPROM_SIZE 1
#define LED_AVAILABLE_EEPROM_ADDR FRIDGE_PAUSE_ON_DOOR_OPEN_ADDR + 1
//Definice EEPROM pro ukládání barvy LED osvětlení 
#define LED_COLOR_EEPROM_SIZE 3
#define LED_COLOR_EEPROM_ADDR LED_AVAILABLE_EEPROM_ADDR + 1
//Definice EEPROM pro ukládání jasu LED osvětlení 
#define LED_BRIGHTNESS_EEPROM_SIZE 1
#define LED_BRIGHTNESS_EEPROM_ADDR LED_COLOR_EEPROM_ADDR + 3


//Definice celkové délky 
#define EEPROM_MAX_SIZE MAC_EEPROM_SIZE + DISPLAY_AVAILABLE_EEPROM_SIZE + POWER_MODE_EEPROM_SIZE + IN_FANS_EEPROM_SIZE + PIEZO_ON_ERROR_SIZE + FRIDGE_PAUSE_ON_DOOR_OPEN_SIZE + LED_AVAILABLE_EEPROM_SIZE + LED_COLOR_EEPROM_SIZE + LED_BRIGHTNESS_EEPROM_SIZE


/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//Definice unikátních ID pro různé služby,

//pro vlastní UUID využijte generátor
//https://www.uuidgenerator.net/
#define SERVICE_UUID           "cea986c2-4405-11ee-be56-0242ac120002" 
#define CHARACTERISTIC_DISPLAY_ENABLE_UUID "cea98ac8-4405-11ee-be56-0242ac120002"
#define CHARACTERISTIC_DHT_INSIDE_UUID "cea98c12-4405-11ee-be56-0242ac120002"
#define CHARACTERISTIC_DHT_OUTSIDE_UUID "cea99162-4405-11ee-be56-0242ac120002"
#define CHARACTERISTIC_DISPLAY_STATE_UUID "52a25b48-4596-11ee-be56-0242ac120002"
#define CHARACTERISTIC_IN_FANS_UUID "615f0ef8-651a-11ee-8c99-0242ac120002"
#define CHARACTERISTIC_POWER_MODE_UUID "c01280b7-3e33-4eb4-ae39-2ec305750179"
#define CHARACTERISTIC_POWER_MODE_CHANGE_UUID "2ee4dfec-e042-4f46-8e2f-46e844b91ae4"
#define CHARACTERISTIC_BUZZING_ON_ERROR_UUID "f639b9d8-6aa1-11ee-8c99-0242ac120002"
#define CHARACTERISTIC_NTC_COOLER_UUID "e67ad112-b64c-445f-8588-d358311d9612"
#define CHARACTERISTIC_UPTIME_UUID "1e95497f-7cff-4376-836e-d6d9b9f1eb7e"
#define CHARACTERISTIC_JSON_DATA_UUID "ddb31e15-aa44-4a42-b3e7-e253f457da2d"
#define CHARACTERISTIC_READY_TO_SEND_JSON_DATA_UUID "4e9a17e0-1c7e-48b8-9a16-7d3a91738ab0"
#define CHARACTERISTIC_FACTORY_UUID "a488b067-27fc-47e6-85b2-22416551775d"
#define CHARACTERISTIC_DOOR_PAUSE_UUID "bd5f15cf-94a7-4d19-b906-64bb570d57be"
#define CHARACTERISTIC_LED_ENABLE_UUID "27537e3a-92d1-11ee-b9d1-0242ac120002"
#define CHARACTERISTIC_LED_COLOR_UUID "27538114-92d1-11ee-b9d1-0242ac120002"
#define CHARACTERISTIC_LED_BRIGHTNESS_UUID "2753827c-92d1-11ee-b9d1-0242ac120002"


//Definice maximální délku datového balíčku
#define MAX_PACKET_SIZE 20;  // Maximální délka balíčku v bajtech
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//Definice výchozích nastavení chytré ledničky 

//Definice, která určuje výchozí dostupnost displeje
#define DISPLAY_DEFAULT_AVAILABLE 1 //V tomto případě je displej při prvním zapnutí zapnutý

//Definice, ktera určuje výchozí stav vnitřních ventilátorů
#define IN_FANS_DEFAULT_AVAILABLE 1 //V tomto případě jsou vnitřní ventilátory při prvním zapnutí zapnutý

//Definice výchozího režimu napájení
#define DEFAULT_POWER_MODE 0 //V tomto případě je lednička vypnutá podle struktury fridge_power_mode

//Definice, která určuje, zda má být piezo aktivní při chybě
#define DEFAULT_PIEZO_ON_ERROR 1 //V tomto případě je piezo aktivní při chybě

//Definice, která určuje, po jaké době se mají ukládat data o naměřených hodnot
#define DEFAULT_JSON_DATA_SAVE_INTERVAL 1000 * 60 //V tomto případě se každou minutu uloží naměřená data do JSON souboru na ESP

//Definice výchozího nastavení, zda se má lednička pozastavit při otevřených dveří
#define DEFAULT_FRIDGE_PAUSE_ON_DOOR_OPEN 1 //V tomto případě se lednička pozastaví při otevřených dveří
//Zde následuje několik definicí ke správci dat pro graf (dataJSONManager) 

//Definice výchozího nastavení LED osvětlení 
#define DEFAULT_LED_ENABLE 1 //V tomto případě se LED osvětlení rozsvítí při otevření dveří 
//Definice výchozího jasu LED osvětlení 
#define DEFAULT_LED_BRIGHTNESS 100 //V tomto případě je jas nastaven na 100%

//Definice názvu souboru pro ukládání dat v grafu
#define DEFAULT_JSON_DATA_FILE_NAME "/fridge_data.json" 
//Definice názvu klíče v JSON vnitřní teploty 
#define DEFAULT_JSON_IN_TEMP_NAME "in_temp"
//Definice názvu klíče v JSON venkovní teploty
#define DEFAULT_JSON_OUT_TEMP_NAME "out_temp"
//Definice názvu klíče v JSON určující teplotu chladiče
#define DEFAULT_JSON_COOLER_TEMP_NAME "cooler_temp"

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//Definice proměnných



//Proměnná pro ukládání zda je zařízení připojené
extern bool devicePaired;

typedef enum {

    FRIDGE_OFF_POWER = 0,
    FRIDGE_MAX_POWER,
    FRIDGE_NORMAL_POWER,
    FRIDGE_ECO_POWER,
    FRIDGE_PAUSED,


} fridge_power_mode;

//Definice struktury pro nastavení chytré ledničky 
struct fridge_data
{
    //Proměnná pro ukládání mac adresy spárovaného zařízení
    BLEAddress* paired_device_address = nullptr;

    //Proměnná pro ukládání vnitřní teploty ledničky ve formátu string 
    String in_temp = "";

    //Proměnná pro ukládání venkovní teploty ve formátu string 
    String out_temp = "";

    //Proměnná pro ukládání teploty chladiče ve formátu string 
    String cooler_temp = "";

    //Proměnná pro uložení zda ve vnitřním teploměru došlo k chybě
    bool in_temp_error = false;

    //Proměnná pro uložení zda ve venkovním teploměru došlo k chybě
    bool out_temp_error = false;
};

//Proměnná pro globální nastavení chytré ledničky 
extern fridge_data FridgeData;

//Předávací deklarace tříd
class FridgeTempDHT;
class ButtonManager;
class PowerManager;
class ThermistorManager;
class DigitalPotentiometer;
class RGBManager;
//Proměnná pro uložení BLE serveru
extern BLEServer* pServer;
//Proměnná pro uložení DHT senzoru vnitřní teploty
extern FridgeTempDHT* inside_temp_dht;
//Proměnná pro uložení DHT senzoru venkovní teploty
extern FridgeTempDHT* outside_temp_dht;
//Proměnná pro uložení NTC senzoru teploty chladiče
extern ThermistorManager* cooler_temp_ntc;
//Proměnná pro uložení třídy resetovacího tlačítka
extern ButtonManager* resetButton;
//Proměnná pro uložení třídy relé chladících ventilátorů
extern RelayModule* relay_cooling_fans;
//Proměnná pro uložení třídy relé vnitřních ventilátorů
extern RelayModule* relay_in_fans;
//Proměnná pro uložení třídy relé hlavního napájení peltierů
extern RelayModule* relay_peltier;
//Proměnná pro uložení třídy relé ovládací režim napájení peltierů
extern RelayModule* relay_peltier_power_mode;
//Proměnná pro uložení třídy digitálního potenciometru
extern DigitalPotentiometer* digital_potentiometer;
//Proměnná pro uložení venkovních chladících PWM ventilátorů
extern FanController<COOLING_FAN_PWM, COOLING_FAN_TACH> cooling_fans_pwm;
//Proměnná pro uložení třídy správce napájení
extern PowerManager* fridge_power_manager;
//Proměnná pro uložení třídy RGB světla
extern RGBManager* fridge_rgb;

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//Definice funkcí 

//Funkce, která vrátí mac adresu spárované zařízení z EEPROM
BLEAddress* read_paired_device_mac_address_from_eeprom();

/////////////////////////////////////////////////////////////////////
