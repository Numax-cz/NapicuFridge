import {FridgeDisplayState, FridgePowerMode} from "./Enums";




export declare interface FridgeData {
  in_temp: string,
  out_temp: string,


  config: {
    fridge_display_available: boolean,
    fridge_display_state: FridgeDisplayState,

    fridge_in_fans: boolean,


    fridge_power_mode: FridgePowerMode,
    fridge_previous_power_mode: FridgePowerMode
  }
}
