import {FridgePowerMode} from "../interface/Enums";
import {Color, ScaleType} from "@swimlane/ngx-charts";
import {CharTempsData} from "../interface/CharData";

export namespace Configuration {

  //Deklarace UUID
  export const SERVICE_UUID: string = "cea986c2-4405-11ee-be56-0242ac120002";
  export const CHARACTERISTIC_DISPLAY_ENABLE_UUID: string = "cea98ac8-4405-11ee-be56-0242ac120002";
  export const CHARACTERISTIC_DHT_INSIDE_UUID: string = "cea98c12-4405-11ee-be56-0242ac120002";
  export const CHARACTERISTIC_DHT_OUTSIDE_UUID: string =  "cea99162-4405-11ee-be56-0242ac120002";
  export const CHARACTERISTIC_DISPLAY_STATE_UUID: string = "52a25b48-4596-11ee-be56-0242ac120002";
  export const CHARACTERISTIC_IN_FANS_UUID: string = "615f0ef8-651a-11ee-8c99-0242ac120002";
  export const CHARACTERISTIC_POWER_MODE_UUID: string =  "c01280b7-3e33-4eb4-ae39-2ec305750179";
  export const CHARACTERISTIC_BUZZING_ON_ERROR_UUID: string = "f639b9d8-6aa1-11ee-8c99-0242ac120002";
  export const CHARACTERISTIC_NTC_COOLER_UUID: string =  "e67ad112-b64c-445f-8588-d358311d9612";
  export const CHARACTERISTIC_UPTIME_UUID: string = "1e95497f-7cff-4376-836e-d6d9b9f1eb7e";
  export const CHARACTERISTIC_JSON_DATA_UUID: string = "ddb31e15-aa44-4a42-b3e7-e253f457da2d";
  export const CHARACTERISTIC_READY_TO_SEND_JSON_DATA_UUID: string = "4e9a17e0-1c7e-48b8-9a16-7d3a91738ab0";
}

//Definice proměnné, která ukládá výchozí režim do kterého lednička přejde po prvním zapnutí
export const DEFAULT_POWER_MODE_ON_SWITCH: FridgePowerMode = FridgePowerMode.FRIDGE_NORMAL_POWER;

export const DEFAULT_IN_FANS_ON_SWITCH: boolean = false;


//Definice výchozího názvu vnitřní teploty
export const CHAR_IN_TEMP_TEXT: string = "Vnitřní teplota";
//Definice výchozího názvu venkovní teploty
export const CHAR_OUT_TEMP_TEXT: string = "Venkovní teplota";
//Definice výchozího názvu teploty chladiče
export const CHAR_COOLER_TEMP_TEXT: string = "Teplota teplota";
//Definice výchozího formátu času poslední aktualizace grafu
export const CHAR_LAST_UPDATE_DATE_FORMAT: string = "%dt.%MM.%yyyy %HH:%mm";

//Definice výchozí barvy křivky znázorňující vnitřní teplotu
export const CHAR_IN_TEMP_COLOR: string = "#00a8ff";
//Definice výchozí barvy křivky znázorňující venkovní teplotu
export const CHAR_OUT_TEMP_COLOR: string = "#4cd137";
//Definice výchozí barvy křivky znázorňující teplotu chladiče
export const CHAR_COOLER_TEMP_COLOR: string = "#e84118";
//Definice výchozí možnosti rozlišení grafu podle proměnné CHAR_VIEW_RESOLUTION_OPTIONS
export const CHAR_DEFAULT_VIEW_RESOLUTION_INDEX: number = 0;
//Definice výchozích hodnot určující kolik maximálně hodnot se má zobrazovat v grafu
export const CHAR_VIEW_RESOLUTION_OPTIONS: number[] = [
  5,
  10,
  30,
  60,
]

//Definice proměnné výchozích dat pro graf, když je aplikace spuštěna ve vývojovém režimu
export const DEFAULT_CHAR_VIEW_DATA_FOR_DEV: CharTempsData = [
  {name: CHAR_IN_TEMP_TEXT, series: [
        { name: "1m", value: 22.4 },
        { name: "2m", value: 23.1 },
        { name: "3m", value: 21.8 },
        { name: "4m", value: 22.9 },
        { name: "5m", value: 21.2 },
        { name: "6m", value: 23.5 },
        { name: "7m", value: 24.0 },
        { name: "8m", value: 20.7 },
        { name: "9m", value: 22.2 },
        { name: "10m", value: 21.9 },
        { name: "11m", value: 24.3 },
        { name: "12m", value: 20.4 },
        { name: "13m", value: 23.8 },
        { name: "14m", value: 22.7 },
        { name: "15m", value: 21.5 },
        { name: "16m", value: 23.2 },
        { name: "17m", value: 22.1 },
        { name: "18m", value: 21.3 },
        { name: "19m", value: 24.5 },
        { name: "20m", value: 20.9 },
        { name: "21m", value: 22.4 },
        { name: "22m", value: 23.1 },
        { name: "23m", value: 21.8 },
        { name: "24m", value: 22.9 },
        { name: "25m", value: 21.2 },
        { name: "26m", value: 23.5 },
        { name: "27m", value: 24.0 },
        { name: "28m", value: 20.7 },
        { name: "29m", value: 22.2 },
        { name: "30m", value: 21.9 },
        { name: "31m", value: 24.3 },
        { name: "32m", value: 20.4 },
        { name: "33m", value: 23.8 },
        { name: "34m", value: 22.7 },
        { name: "35m", value: 21.5 },
        { name: "36m", value: 23.2 },
        { name: "37m", value: 22.1 },
        { name: "38m", value: 21.3 },
        { name: "39m", value: 24.5 },
        { name: "40m", value: 20.9 },
        { name: "41m", value: 22.4 },
        { name: "42m", value: 23.1 },
        { name: "43m", value: 21.8 },
        { name: "44m", value: 22.9 },
        { name: "45m", value: 21.2 },
        { name: "46m", value: 23.5 },
        { name: "47m", value: 24.0 },
        { name: "48m", value: 20.7 },
        { name: "49m", value: 22.2 },
        { name: "50m", value: 21.9 },
        { name: "51m", value: 24.3 },
        { name: "52m", value: 20.4 },
        { name: "53m", value: 23.8 },
        { name: "54m", value: 22.7 },
        { name: "55m", value: 21.5 },
        { name: "56m", value: 23.2 },
        { name: "57m", value: 22.1 },
        { name: "58m", value: 21.3 },
        { name: "59m", value: 24.5 },
        { name: "60m", value: 20.9 },
    ]},
  {name: CHAR_OUT_TEMP_TEXT, series: [
        { name: "1m", value: 22.6 },
        { name: "2m", value: 21.0 },
        { name: "3m", value: 23.9 },
        { name: "4m", value: 21.7 },
        { name: "5m", value: 22.3 },
        { name: "6m", value: 24.1 },
        { name: "7m", value: 23.6 },
        { name: "8m", value: 21.4 },
        { name: "9m", value: 20.5 },
        { name: "10m", value: 23.4 },
        { name: "11m", value: 22.8 },
        { name: "12m", value: 21.6 },
        { name: "13m", value: 22.0 },
        { name: "14m", value: 24.2 },
        { name: "15m", value: 23.3 },
        { name: "16m", value: 20.8 },
        { name: "17m", value: 22.5 },
        { name: "18m", value: 21.2 },
        { name: "19m", value: 23.7 },
        { name: "20m", value: 24.4 },
        { name: "21m", value: 22.6 },
        { name: "22m", value: 21.0 },
        { name: "23m", value: 23.9 },
        { name: "24m", value: 21.7 },
        { name: "25m", value: 22.3 },
        { name: "26m", value: 24.1 },
        { name: "27m", value: 23.6 },
        { name: "28m", value: 21.4 },
        { name: "29m", value: 20.5 },
        { name: "30m", value: 23.4 },
        { name: "31m", value: 22.8 },
        { name: "32m", value: 21.6 },
        { name: "33m", value: 22.0 },
        { name: "34m", value: 24.2 },
        { name: "35m", value: 23.3 },
        { name: "36m", value: 20.8 },
        { name: "37m", value: 22.5 },
        { name: "38m", value: 21.2 },
        { name: "39m", value: 23.7 },
        { name: "40m", value: 24.4 },
        { name: "41m", value: 22.4 },
        { name: "42m", value: 23.1 },
        { name: "43m", value: 21.8 },
        { name: "44m", value: 22.9 },
        { name: "45m", value: 21.2 },
        { name: "46m", value: 23.5 },
        { name: "47m", value: 24.0 },
        { name: "48m", value: 20.7 },
        { name: "49m", value: 22.2 },
        { name: "50m", value: 21.9 },
        { name: "51m", value: 24.3 },
        { name: "52m", value: 20.4 },
        { name: "53m", value: 23.8 },
        { name: "54m", value: 22.7 },
        { name: "55m", value: 21.5 },
        { name: "56m", value: 23.2 },
        { name: "57m", value: 22.1 },
        { name: "58m", value: 21.3 },
        { name: "59m", value: 24.5 },
        { name: "60m", value: 20.9 },
    ]},
  {name: CHAR_COOLER_TEMP_TEXT,
    series: [
        { name: "1m", value: 22.6 },
        { name: "2m", value: 21.0 },
        { name: "3m", value: 23.9 },
        { name: "4m", value: 21.7 },
        { name: "5m", value: 22.3 },
        { name: "6m", value: 24.1 },
        { name: "7m", value: 23.6 },
        { name: "8m", value: 21.4 },
        { name: "9m", value: 20.5 },
        { name: "10m", value: 23.4 },
        { name: "11m", value: 22.8 },
        { name: "12m", value: 21.6 },
        { name: "13m", value: 22.0 },
        { name: "14m", value: 24.2 },
        { name: "15m", value: 23.3 },
        { name: "16m", value: 20.8 },
        { name: "17m", value: 22.5 },
        { name: "18m", value: 21.2 },
        { name: "19m", value: 23.7 },
        { name: "20m", value: 24.4 },
        { name: "21m", value: 22.6 },
        { name: "22m", value: 21.0 },
        { name: "23m", value: 23.9 },
        { name: "24m", value: 21.7 },
        { name: "25m", value: 22.3 },
        { name: "26m", value: 24.1 },
        { name: "27m", value: 23.6 },
        { name: "28m", value: 21.4 },
        { name: "29m", value: 20.5 },
        { name: "30m", value: 23.4 },
        { name: "31m", value: 22.8 },
        { name: "32m", value: 21.6 },
        { name: "33m", value: 22.0 },
        { name: "34m", value: 24.2 },
        { name: "35m", value: 23.3 },
        { name: "36m", value: 20.8 },
        { name: "37m", value: 22.5 },
        { name: "38m", value: 21.2 },
        { name: "39m", value: 23.7 },
        { name: "40m", value: 24.4 },
        { name: "41m", value: 22.4 },
        { name: "42m", value: 23.1 },
        { name: "43m", value: 21.8 },
        { name: "44m", value: 22.9 },
        { name: "45m", value: 21.2 },
        { name: "46m", value: 23.5 },
        { name: "47m", value: 24.0 },
        { name: "48m", value: 20.7 },
        { name: "49m", value: 22.2 },
        { name: "50m", value: 21.9 },
        { name: "51m", value: 24.3 },
        { name: "52m", value: 20.4 },
        { name: "53m", value: 23.8 },
        { name: "54m", value: 22.7 },
        { name: "55m", value: 21.5 },
        { name: "56m", value: 23.2 },
        { name: "57m", value: 22.1 },
        { name: "58m", value: 21.3 },
        { name: "59m", value: 24.5 },
        { name: "60m", value: 20.9 },

    ]}]
