import {FridgePowerMode} from "../interface/Enums";

export namespace Configuration {

  //Deklarace UUID
  export const SERVICE_UUID: string = "cea986c2-4405-11ee-be56-0242ac120002";
  export const CHARACTERISTIC_DISPLAY_ENABLE_UUID: string = "cea98ac8-4405-11ee-be56-0242ac120002";
  export const CHARACTERISTIC_DHT_INSIDE_UUID: string = "cea98c12-4405-11ee-be56-0242ac120002";
  export const CHARACTERISTIC_DHT_OUTSIDE_UUID: string =  "cea99162-4405-11ee-be56-0242ac120002";
  export const CHARACTERISTIC_DISPLAY_STATE_UUID: string = "52a25b48-4596-11ee-be56-0242ac120002";
  export const CHARACTERISTIC_IN_FANS_UUID: string = "615f0ef8-651a-11ee-8c99-0242ac120002";
  export const  CHARACTERISTIC_POWER_MODE_UUID: string =  "c01280b7-3e33-4eb4-ae39-2ec305750179";
  export const CHARACTERISTIC_BUZZING_ON_ERROR_UUID: string = "f639b9d8-6aa1-11ee-8c99-0242ac120002";
  export const CHARACTERISTIC_NTC_COOLER_UUID: string =  "e67ad112-b64c-445f-8588-d358311d9612";
}

//Definice proměnné, která ukládá výchozí režim do kterého lednička přejde po prvním zapnutí
export const DEFAULT_POWER_MODE_ON_SWITCH: FridgePowerMode = FridgePowerMode.FRIDGE_NORMAL_POWER;

export const DEFAULT_IN_FANS_ON_SWITCH: boolean = false;
