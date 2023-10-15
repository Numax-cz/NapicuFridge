import {FridgeDisplayState, FridgePowerMode} from "./Enums";




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
}
