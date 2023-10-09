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

//Připojení potřebných knihoven z include
#include <include/serverCallBack.h>
#include <include/fridgeTempDHT.h>
#include <include/fridgeDisplay.h>
#include <include/relayModule.h>
#include <include/buttonManager.h>
#include <include/fanController.h>
#include <include/powerManager.h>

/////////////////////////////////////////////////////////////////////
//Definice propojovacích pinů

//pro analogový vstup a LED diodu
#define CONNECTION_LED 19
//Definice pinu pro PIEZO
#define PIEZO_PIN 18
//Definice propojovacích pinů veškerých teploměrů 
#define DHT_TYPE DHT11 //Typ DHT senzoru
#define DHT_INSIDE 17  // Pin vnitřního DHT senzoru
#define DHT_OUTSIDE 16 //Pin venkovního DHT senzoru

//Definice pinu tlačítka pro reset zařízení
#define RESET_BUTTON 5

//Definice pinů digitálního potenciometru
#define X9_CS  14 //Pin CS pinu potenciometru
#define X9_INC 27 //Pin INC pinu potenciometru
#define X9_UD  26 //Pin UD pinu potenciometru 

//Definice pinů termistoru
#define SENSOR_PIN             4 //Pin pro příjem dat
#define REFERENCE_RESISTANCE    9830 // Referenční hodnota rezistoru 
#define NOMINAL_RESISTANCE     7400 // Nominální odpor
#define NOMINAL_TEMPERATURE    26 //Nominální teplota 
#define B_VALUE                3950 //beta hodnota 
#define STM32_ANALOG_RESOLUTION 4095 //Analogové Rozlišení 

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
//Definice celkové délky 
#define EEPROM_MAX_SIZE MAC_EEPROM_SIZE + DISPLAY_AVAILABLE_EEPROM_SIZE + POWER_MODE_EEPROM_SIZE + IN_FANS_EEPROM_SIZE


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

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//Definice výchozích nastavení chytré ledničky 

//Definice, která určuje výchozí dostupnost displeje
#define DISPLAY_DEFAULT_AVAILABLE 1 //V tomto případě je displej při prvním zapnutí zapnutý

//Definice, ktera určuje výchozí stav vnitřních ventilátorů
#define IN_FANS_DEFAULT_AVAILABLE 1 //V tomto případě jsou vnitřní ventilátory při prvním zapnutí zapnutý

//Definice výchozího režimu napájení
#define DEFAULT_POWER_MODE 0 //V tomto případě je lednička vypnutá podle struktury fridge_power_mode

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
};

//Proměnná pro globální nastavení chytré ledničky 
extern fridge_data FridgeData;

//Předávací deklarace tříd
class FridgeTempDHT;
class ButtonManager;
class PowerManager;
//Proměnná pro uložení BLE serveru
extern BLEServer* pServer;
//Proměnná pro uložení DHT senzoru vnitřní teploty
extern FridgeTempDHT* insideTempDHT;
//Proměnná pro uložení DHT senzoru venkovní teploty
extern FridgeTempDHT* outsideTempDHT;
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
//Proměnná pro uložení třídy termistoru pro zaznamenávaní teploty teplé strany chladiče
extern Thermistor* out_thermistor;
//Proměnná pro uložení třídy digitálního potenciometru
extern DigiPot* digitalPotentiometer;
//Proměnná pro uložení venkovních chladících PWM ventilátorů
extern FanController<COOLING_FAN_PWM, COOLING_FAN_TACH> cooling_fans_pwm;
 //Proměnná pro uložení třídy správce napájení
extern PowerManager* fridge_power_manager;

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//Definice funkcí 

//Funkce, která vrátí mac adresu spárované zařízení z EEPROM
BLEAddress* read_paired_device_mac_address_from_eeprom();

//Funkce, která uvede zařízení do továrního nastavení
void factory_reset();

/////////////////////////////////////////////////////////////////////
