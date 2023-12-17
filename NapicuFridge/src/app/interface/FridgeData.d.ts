import {FridgeDisplayState, FridgePowerMode} from "./Enums";
import {CharTempsData} from "./CharData";
import {NapicuOptionsData} from "./NapicuOption";
import {RGB, RGBA} from "ngx-color/helpers/color.interfaces";


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

    buzzing_on_error: boolean,
    fridge_stop_on_open_door: boolean


    fridge_power_mode: FridgePowerMode,

    fridge_led_enable: boolean,
    fridge_led_rgb: RGBA,
    fridge_led_brightness: number
  }

  errors: {
    fridge_in_temp: boolean,
    fridge_out_temp: boolean,
    fridge_cooler_temp: boolean
  }

  user_favorites_colors: RGB[],
  user_delete_color_hint: boolean
  char_settings: CharSettings,
  json_graph_chars_format: CharTempsData | null,
  json_graph_chars_format_view: CharTempsData |null,
  json_graph_resolution_view: NapicuOptionsData[] | null
}
