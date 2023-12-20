import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {LabelType, Options} from "ngx-slider-v2";
import {CharacteristicController} from "../../../CharacteristicController";
import {ColorEvent} from "ngx-color";
import {RGB, RGBA} from "ngx-color/helpers/color.interfaces";
import {ChangeContext} from "ngx-slider-v2/change-context";
import {MIN_BRIGHTNESS_SLIDER_VALUE} from "../../../config/configuration";
import {alert_animations, favourite_color_animations} from "../../Animation";


@Component({
  selector: 'app-information',
  templateUrl: './lighting.component.html',
  styleUrls: ['./lighting.component.scss', "../../main.page.scss"],
  animations: [favourite_color_animations, alert_animations]
})
export class LightingComponent  {
  //Proměnná pro nastavení slideru
  public options: Options = {
    disabled: !AppComponent.get_fridge_led_enable(),
    floor: MIN_BRIGHTNESS_SLIDER_VALUE,
    ceil: 100,
      showSelectionBarEnd: true,
      animate: false,
  };

  //Proměnná ukládající čas aktuálního držení oblíbené barvy
  public timeoutHandler: number | undefined = undefined;
  //Proměnná ukládající stav animace
  public trigger: boolean = true;
  //Porměnná ukládající index držící barvy
  public holding_color: number = -1;
  //Proměnná ukládající interval animace držící barvy
  public shaking_color_interval: number = -1;
  //Proměnná pro uložení, zda je aktivován odstraňovací režim oblíbených barev
  public delete_color_mode: boolean = false;
  //Proměnná pro uložení stavu alertu
  public hint_alert: boolean = false;

  constructor(public ngZone: NgZone) { }

  //Funkce, která se spustí po změně inputu
  public led_input_change(event: any): void {
    const i: boolean = event.currentTarget.checked;
    //Aktualizování nastavení slideru
    this.options = Object.assign({}, this.options, {disabled: !i});
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
    if(!this.get_is_connected() || !this.get_fridge_led_enable()) return
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
    if(!this.get_is_connected() || !this.get_fridge_led_enable()) return
    //Spuštění funkce pro zápis charakteristiky na změnu hodnoty jasu LED osvětlení
    CharacteristicController.writeLEDBrightness(event.value)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
    });
  }

  //Funkce, která přidá barvu do oblíbených barev osvětlení. Tato funkce se spustí po kliknutí na oblíbenou barvu
  public on_click_favorite_color(color: RGB): void {
    if(!this.get_is_connected() || !this.get_fridge_led_enable()) return
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
    if(!this.get_is_connected() || !this.get_fridge_led_enable()) return
    //Nastavení indexu držící barvy
    this.holding_color = element_index;
    //Uložení timout
    this.timeoutHandler = setTimeout(() => {
      //Pokud není aktivován režim odstraňování oblíbených barev provede se následující
      if(!this.delete_color_mode) {
        //Uložení intervalu do proměnné
        this.shaking_color_interval = setInterval(() => (this.trigger = !this.trigger),200);
        //Spuštění Timout funkce, následně nastavení držící barvy na -1
        setTimeout(() => {this.holding_color = -1}, 150);
        //Nastavení proměnné na log1
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
    if(!this.get_is_connected() || !this.get_fridge_led_enable()) return
    //Přidání oblíbené barvy do seznamu oblíbených
    AppComponent.add_user_favorite_color(AppComponent.fridge_data.config.fridge_led_rgb);
    //Pokud je povoleno zobrazování nápovědy provede se následující
    if(AppComponent.get_is_delete_color_hint_enabled()) {
      //Nastavení proměnné pro zobrazení nápovědy na log1
      this.hint_alert = true;
    }
  }

  //Funkce, která vypne zobrazování nápovědy k odstranění oblíbené barvy
  public disable_favorites_colors_hint(): void {
    AppComponent.disable_favorites_colors_hint();
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
