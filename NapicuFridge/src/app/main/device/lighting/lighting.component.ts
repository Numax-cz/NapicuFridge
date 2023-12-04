import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {LabelType, Options} from "ngx-slider-v2";


@Component({
  selector: 'app-information',
  templateUrl: './lighting.component.html',
  styleUrls: ['./lighting.component.scss', "../../main.page.scss"],
})
export class LightingComponent  {

  value: number = 50;
  options: Options = {
    floor: 0,
    ceil: 100,
      showSelectionBarEnd: true,
      animate: false,

    // translate: (value: number, label: LabelType): string => {
    //   switch (label) {
    //     case LabelType.Low:
    //       return value + "%";
    //     case LabelType.High:
    //       return value + "%";
    //     default:
    //       return value.toString();
    //   }
    // }
  };

  constructor(public ngZone: NgZone) {
  }

  //Funkce, která se spustí po změně inputu
  public led_input_change(event: any): void {

  }

  //Funkce, která vrátí zda je zařízení připojené
  public get_is_connected(): boolean {
    return AppComponent.is_connected();
  }


}
