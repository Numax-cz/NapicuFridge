import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {LabelType, Options} from "ngx-slider-v2";
import {CharacteristicController} from "../../../CharacteristicController";
import {ColorEvent} from "ngx-color";
import {RGB, RGBA} from "ngx-color/helpers/color.interfaces";
import {ChangeContext} from "ngx-slider-v2/change-context";
import {MIN_BRIGHTNESS_SLIDER_VALUE} from "../../../config/configuration";
import {favourite_color_animations} from "../../Animation";


@Component({
  selector: 'app-information',
  templateUrl: './lighting.component.html',
  styleUrls: ['./lighting.component.scss', "../../main.page.scss"],
  animations: favourite_color_animations
})
export class LightingComponent  {
  //Proměnná ukládající čas aktuálního držení oblíbené barvy
  public timeoutHandler: number | undefined = undefined;

  //Proměnná pro nastavení slideru
  public readonly options: Options = {
    floor: MIN_BRIGHTNESS_SLIDER_VALUE,
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
  public trigger: boolean = true;

  public holding_color: number = -1;

  public shaking_color_interval: number = -1;

  public delete_color_mode: boolean = false;

  constructor(public ngZone: NgZone) {

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

  //Funkce, která se spustí při změně barvy v color pickeru
  public on_color_input_change(color: RGBA): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      //Zapíšeme aktuální barvu LED osvětlení
      AppComponent.fridge_data.config.fridge_led_rgb = {
        r: color.r,
        g: color.g,
        b: color.b,
        a: 255
      }
    });
  }

  //Funkce, která se spustí po výběru barvy v color pickeru
  public write_color_value(color: RGBA): void {
    //Spuštění funkce pro zápis charakteristiky na změnu barvy LED osvětlení
    CharacteristicController.writeLEDEColor(color.r, color.g, color.b)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
    });
  }

  //Funkce, která se spustí při změně hodnoty v inputu
  public on_brightness_input_change(value: number): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      //Zapíšeme aktuální dostupnost LED osvětlení
      AppComponent.fridge_data.config.fridge_led_brightness = value;
    });
  }

  //Funkce, která se spustí při výběru hodnoty v inputu
  public write_brightness_value(event: ChangeContext): void {
    //Spuštění funkce pro zápis charakteristiky na změnu hodnoty jasu LED osvětlení
    CharacteristicController.writeLEDBrightness(event.value)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
    });
  }

  //Funkce, která přidá barvu do oblíbených barev osvětlení. Tato funkce se spustí po kliknutí na oblíbenou barvu
  public on_click_favorite_color(color: RGB): void {
    this.holding_color = -1;
    clearTimeout(this.timeoutHandler);
    if(!this.delete_color_mode) {
      //Spuštění funkce pro změnu barvy
      this.on_color_input_change({...color, a: 255});
      //Spuštění funkce pro zápis barvy do ESP
      this.write_color_value({...color, a: 255});
    }
  }

  //Funkce, která ukončí mazací režim oblíbených barev
  public exit_delete_mode(): void {
    clearInterval(this.shaking_color_interval);
    this.delete_color_mode = false;
  }

  //Funkce, která se spustí po okamžitém kliknutí na oblíbenou barvu
  public on_touch_favorite_color(event: Event, element_index: number): void {
    this.holding_color = element_index;
      this.timeoutHandler = setTimeout(() => {
        if(!this.delete_color_mode) {
          this.shaking_color_interval = setInterval(() => (this.trigger = !this.trigger),200);
          setTimeout(() => {this.holding_color = -1}, 150);
          this.delete_color_mode = true;
        } else {
          this.holding_color = -1;
        }
        event.preventDefault();
      }, 600);
  }

  //Funkce, která vrátí z typu RGB formát pro css styl ve formátu string
  public get_css_color_format(color: RGB): string {
    return `rgb(${color.r}, ${color.g}, ${color.b})`
  }

  //Funkce, která přidá barvu do oblíbených barev osvětlení
  public add_user_favorite_color(): void {
    AppComponent.add_user_favorite_color(AppComponent.fridge_data.config.fridge_led_rgb);
  }

  //Funkce, která odebere oblíbenou barvu osvětlení podle indexu
  public remove_user_favorite_color(index: number): void {
    AppComponent.remove_user_favorite_color(index);
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
  //Funkce, která vrátí hodnotu jasu LED osvětlení (0-100)
  public get_fridge_led_brightness(): number {
    return AppComponent.get_fridge_led_brightness();
  }

  //Funkce, která vrátí oblíbené barvy
  public get_user_favorites_colors(): RGB[] {
    return AppComponent.get_user_favorites_colors();
  }
}
