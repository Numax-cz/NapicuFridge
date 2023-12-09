import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {LabelType, Options} from "ngx-slider-v2";
import {CharacteristicController} from "../../../CharacteristicController";
import {ColorEvent} from "ngx-color";


@Component({
  selector: 'app-information',
  templateUrl: './lighting.component.html',
  styleUrls: ['./lighting.component.scss', "../../main.page.scss"],
})
export class LightingComponent  {

  public color: string = "#fff";

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
    console.log(event);
    this.color = event.color.hex;
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
    AppComponent.add_user_favorite_color(this.color);
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
  public get_user_favorites_colors(): string[] {
    return AppComponent.get_user_favorites_colors();
  }

}
