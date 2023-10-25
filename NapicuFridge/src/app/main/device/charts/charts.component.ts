import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {CharTempsData} from "../../../interface/CharData";
import {CharSettings} from "../../../interface/FridgeData";
import {ViewWillLeave} from "@ionic/angular";
import {Color, ScaleType} from "@swimlane/ngx-charts";
import {
  CHAR_COOLER_TEMP_COLOR,
  CHAR_IN_TEMP_COLOR,
  CHAR_OUT_TEMP_COLOR
} from "../../../config/configuration";


@Component({
  selector: 'app-charts',
  templateUrl: './charts.component.html',
  styleUrls: ['./charts.component.scss'],
})
export class ChartsComponent implements ViewWillLeave{

  //Definice barev křivek pro graf
  public readonly char_color_schema: Color = {
    name: "color",
    selectable: false,
    group: ScaleType.Linear,
    domain: [
      CHAR_IN_TEMP_COLOR,
      CHAR_OUT_TEMP_COLOR,
      CHAR_COOLER_TEMP_COLOR,
    ]
  };


  constructor(private ngZone: NgZone) {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      //Spuštění funkce pro aktualizovaní dat, které se mají zobrazit v grafu
      AppComponent.update_char_view_data();
    });
  }

  public ionViewWillLeave() {
    AppComponent.clear_char_view_data();
  }

  //Funkce, která vrátí data naměřených teplot, které se mají zobrazit v grafu
  public get_char_view_data(): CharTempsData | null {
    return AppComponent.get_char_view_data();
  }

  //Funkce, která nastaví obrácenou bool hodnotu proměnné určující zobrazení křivky vnitřní teploty na grafu
  public switch_in_temp_display_char(): void {
    //Pokud jsou dostupné data naměřených teplot, provede se následující
    if(this.get_char_view_data()?.length) {
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        AppComponent.switch_in_temp_display_char();
      });
    }
  }

  //Funkce, která nastaví obrácenou bool hodnotu proměnné určující zobrazení křivky venkovní teploty na grafu
  public switch_out_temp_display_char(): void {
    //Pokud jsou dostupné data naměřených teplot, provede se následující
    if(this.get_char_view_data()?.length) {
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        AppComponent.switch_out_temp_display_char();
      });
    }
  }

  //Funkce, která nastaví obrácenou bool hodnotu proměnné určující zobrazení křivky teploty na chladiči v grafu
  public switch_cooler_temp_display_char(): void {
    //Pokud jsou dostupné data naměřených teplot, provede se následující
    if(this.get_char_view_data()?.length) {
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        AppComponent.switch_cooler_temp_display_char();
      });
    }
  }

  //Funkce, která vrátí nastavení grafu
  public get_char_settings(): CharSettings {
      return AppComponent.get_char_settings();
  }

  //Funkce která vrátí čas ve kterém se naposledy aktualizoval graf naměřených hodnot v základním formátu
  public get_char_last_update_basic_format(): string {
    return AppComponent.get_char_last_update_basic_format();
  }

  //Funkce, která vrátí výchozí barvu křivky znázorňující vnitřní teplotu
  public get_char_in_temp_color(): string {
    return CHAR_IN_TEMP_COLOR;
  }

  //Funkce, která vrátí výchozí barvu křivky znázorňující venkovní teplotu
  public get_char_out_temp_color(): string {
    return CHAR_OUT_TEMP_COLOR;
  }

  //Funkce, která vrátí výchozí barvu křivky znázorňující teplotu chladiče
  public get_char_cooler_temp_color(): string {
    return CHAR_COOLER_TEMP_COLOR;
  }
}
