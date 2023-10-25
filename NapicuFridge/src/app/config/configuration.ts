import {FridgePowerMode} from "../interface/Enums";
import {Color, ScaleType} from "@swimlane/ngx-charts";

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
  60
]
