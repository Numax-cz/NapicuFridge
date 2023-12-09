import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {LabelType, Options} from "ngx-slider-v2";
import {CharacteristicController} from "../../../CharacteristicController";
import {ColorEvent} from "ngx-color";
import {RGBA} from "ngx-color/helpers/color.interfaces";


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

  //Funkce, která se spustí při změně barvy v color pickeru
  public change_color(event: ColorEvent): void {
    //Spuštění funkce pro zápis charakteristiky na změnu barvy LED osvětlení
    CharacteristicController.writeLEDEColor(event.color.rgb.r, event.color.rgb.g, event.color.rgb.b)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Zapíšeme aktuální barvu LED osvětlení
        AppComponent.fridge_data.config.fridge_led_rgb = {
          r: event.color.rgb.r,
          g: event.color.rgb.g,
          b: event.color.rgb.b,
          a: 255
        }
      });
    });
  }

  //Funkce, která se spustí po změně inputu
  public led_input_change(event: any): void {
    const i: boolean = event.currentTarget.checked;
    //Spuštění funkce pro zápis charakteristiky na stav LED osvětlení
    CharacteristicController.writeLEDEnable(i)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Zapíšeme aktuální dostupnost LED osvětlení
        AppComponent.fridge_data.config.fridge_led_enable = i;
      });
    });
  }

  //Funkce, která přidá barvu do oblíbených barev osvětlení
  public add_user_favorite_color(): void {

  }

  //Statická funkce, která vrátí barvu LED osvětlení
  public get_led_color(): RGBA{
    return AppComponent.get_led_color();
  }

  //Funkce, která vrátí zda je zařízení připojené
  public get_is_connected(): boolean {
    return AppComponent.is_connected();
  }

  //Funkce, která vrátí zda se má LED osvětlení zapnout při otevřených dveří
  public get_fridge_led_enable(): boolean {
    return AppComponent.get_fridge_led_enable();
  }

  //Funkce, která vrátí oblíbené barvy
  public get_user_favorites_colors(): any {

  }

}
