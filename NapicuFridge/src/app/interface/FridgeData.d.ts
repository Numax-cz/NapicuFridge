import {FridgeDisplayState, FridgePowerMode} from "./Enums";
import {CharTempsData} from "./CharData";


export declare interface FridgeJSONData {
  in_temp: number[];
  out_temp: number[];
  cooler_temp: number[];
}

export declare interface CharSettings {
  display_in_temp: boolean,
  display_out_temp: boolean,
  display_cooler_temp: boolean,
  display_resolution: number
}

export declare interface FridgeData {
  in_temp: string,
  out_temp: string,
  cooler_temp: string

  config: {
    fridge_display_available: boolean,
    fridge_display_state: FridgeDisplayState,

    fridge_in_fans: boolean,

    buzzing_on_error: boolean


    fridge_power_mode: FridgePowerMode,
  }

  errors: {
    fridge_in_temp: boolean,
    fridge_out_temp: boolean,
    fridge_cooler_temp: boolean
  }

  char_settings: CharSettings,
  json_graph_chars_format: CharTempsData | null,
  json_graph_chars_format_view: CharTempsData |null,
  json_graph_resolution_view: string[] | null
}
