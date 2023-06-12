#pragma once
#include <Arduino.h>

// definice propojovacích pinů
// pro analogový vstup a LED diodu
#define readPin 32
#define CONNECTION_LED 19
#define TEST_LED 18

// definice unikátních ID pro různé služby,
// pro vlastní UUID využijte generátor
// https://www.uuidgenerator.net/
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" //TODO change
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


// proměnná pro kontrolu připojených zařízení
extern bool zarizeniPripojeno;
