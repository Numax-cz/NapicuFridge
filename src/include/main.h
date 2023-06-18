#pragma once
#include <Arduino.h>

// definice propojovacích pinů
// pro analogový vstup a LED diodu
#define readPin 32
#define CONNECTION_LED 19
#define TEST_LED 18

// definice propojovacích pinů veškerých teploměrů 
#define DHT_TYPE DHT11  
#define DHT_INSIDE 17 

// definice unikátních ID pro různé služby,
// pro vlastní UUID využijte generátor
// https://www.uuidgenerator.net/
#define SERVICE_UUID           "5ff8dace-0914-11ee-be56-0242ac120002" 
#define CHARACTERISTIC_UUID_RX "5ff8dda8-0914-11ee-be56-0242ac120002"
#define CHARACTERISTIC_DHT_INSIDE_TX "5ff8e046-0914-11ee-be56-0242ac120002"


// proměnná pro kontrolu připojených zařízení
extern bool devicePaired;
