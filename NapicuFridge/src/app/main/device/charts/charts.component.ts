import {Component, ElementRef, NgZone, ViewChild} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {CharTempsData} from "../../../interface/CharData";
import {CharSettings} from "../../../interface/FridgeData";
import {ViewDidEnter, ViewWillEnter, ViewWillLeave} from "@ionic/angular";
import {Color, ScaleType} from "@swimlane/ngx-charts";
import {
  CHAR_COOLER_TEMP_COLOR,
  CHAR_IN_TEMP_COLOR,
  CHAR_OUT_TEMP_COLOR, DEFAULT_ALERT_DISPLAY_TIME, DEFAULT_CHAR_VIEW_DATA_FOR_DEV
} from "../../../config/configuration";
import {alert_animations, arrows_expand_animations} from "../../Animation";
import {NapicuOptionsData} from "../../../interface/NapicuOption";
import {CharacteristicController} from "../../../CharacteristicController";
import {environment} from "../../../../environments/environment";


@Component({
  selector: 'app-charts',
  templateUrl: './charts.component.html',
  styleUrls: ['./charts.component.scss'],
  animations: [arrows_expand_animations, alert_animations]
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


  //TODO DOC
  public expanded = false;

  //TODO DOC
  public copy_alert_display: boolean = false;

  //Proměnná pro uložení stavu alertu
  public hint_alert: boolean = true;


  constructor(private ngZone: NgZone) {
    //Získání informace, zda se má nápověda zobrazit
    this.hint_alert = AppComponent.get_is_charts_hint_enabled()

    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      //Spuštění funkce pro aktualizovaní dat, které se mají zobrazit v grafu
      AppComponent.update_char_view_data();
    });
  }

  public ionViewWillLeave() {
    AppComponent.clear_char_view_data();
  }
  //Funkce, která vrátí index jaká hodnota je vybraná v CHAR_VIEW_RESOLUTION_OPTIONS
  public get_char_resolution_index(): number {
    return AppComponent.get_char_resolution_index();
  }

  //Funkce, která nastaví kolit dat naměřených teplot, se mají zobrazit v grafu
  public set_char_resolution = (index: number): void => {
    AppComponent.set_char_resolution(index);
  }

  //Funkce, která vrátí data naměřených teplot, které se mají zobrazit v grafu
  public get_char_view_data(): CharTempsData | null {
    return AppComponent.get_char_view_data();
  }
  //Funkce, která vrátí celý json graf naměřených teplot
  public get_full_json_temp_char(): CharTempsData | null {
    return AppComponent.get_full_json_temp_char()
  }

  //Funkce, která nastaví obrácenou bool hodnotu proměnné určující zobrazení křivky vnitřní teploty na grafu
  public switch_in_temp_display_char(): void {
    //Pokud jsou dostupné data naměřených teplot, provede se následující
    if(this.get_full_json_temp_char()?.length) {
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        AppComponent.switch_in_temp_display_char();
      });
    }
  }

  //Funkce, která nastaví obrácenou bool hodnotu proměnné určující zobrazení křivky venkovní teploty na grafu
  public switch_out_temp_display_char(): void {
    //Pokud jsou dostupné data naměřených teplot, provede se následující
    if(this.get_full_json_temp_char()?.length) {
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        AppComponent.switch_out_temp_display_char();
      });
    }
  }

  //Funkce, která nastaví obrácenou bool hodnotu proměnné určující zobrazení křivky teploty na chladiči v grafu
  public switch_cooler_temp_display_char(): void {
    //Pokud jsou dostupné data naměřených teplot, provede se následující
    if (this.get_full_json_temp_char()?.length) {
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        AppComponent.switch_cooler_temp_display_char();
      });
    }
  }

  //Funkce, která vrátí dostupná rozlišení grafu podle velikosti naměřených hodnot
  public get_char_available_resolutions(): NapicuOptionsData[] {
    return AppComponent.get_char_available_resolutions();
  }

  //Funkce, která zkopíruje všechna naměřená data do schránky zařízení
  public copy_json_data_to_clipboard(): void {
    //Zavolání funkce pro kopírování naměřených hodnot z grafu
    AppComponent.copy_json_data_to_clipboard()?.then(() => {
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Nastavení proměnné na log1
        this.copy_alert_display = true;
        //Spuštění funkce pro vykonaní funkce po dobu definovanou proměnnou DEFAULT_ALERT_DISPLAY_TIME
        setTimeout(() => {
          //Nastavení proměnné na log0
          this.copy_alert_display = false;
        }, DEFAULT_ALERT_DISPLAY_TIME);
      });
    });
  }

  //Funkce, která vymaže graf naměřených hodnot
  public delete_char(): void {
    //Spuštění funkce pro zápis charakteristiky na vymazání naměřených hodnot
    CharacteristicController.deleteJSONData()?.then(() => {
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        AppComponent.fridge_data.json_graph_chars_format = null;
        AppComponent.fridge_data.json_graph_chars_format_view = null;
      });
    });
  }

  //Funkce, která vypne zobrazování nápovědy ke grafu
  public disable_charts_hint(): void {
    AppComponent.disable_charts_hint();
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

  //Funkce, která vrátí zda je alert zobrazen
  public get_copy_alert_display(): boolean {
    return this.copy_alert_display;
  }
}
