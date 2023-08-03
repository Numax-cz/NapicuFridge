import {FridgeDisplayState} from "./Enums";

export declare interface FridgeData {
  in_temp: string,


  config: {
    fridge_display_available: boolean,
    fridge_display_state: FridgeDisplayState
  }
}
