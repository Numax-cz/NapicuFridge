import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../app.component";
import {alert_animations} from "../Animation";
import {CharTempsData} from "../../interface/CharData";
import {Color, ScaleType} from "@swimlane/ngx-charts";
import {CHAR_COOLER_TEMP_COLOR, CHAR_IN_TEMP_COLOR, CHAR_OUT_TEMP_COLOR} from "../../config/configuration";

@Component({
  selector: 'app-info',
  templateUrl: './info.page.html',
  styleUrls: ['./info.page.scss', '../main.page.scss'],
  animations: alert_animations
})
export class InfoPage {
  //Proměnná, která určuje zda je informační alert zobrazen
  public is_info_icon_alert_activated: boolean = false;

  protected readonly default_preview_graph_data = [
    {
      "name": "Vnitřní teplota",
      "series": [
        {
          "value": 0,
          "name": "1m"
        },
        {
          "value": 0,
          "name": "2m"
        },
      ]
    },
  ]

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



  constructor(protected ngZone: NgZone) { }

  //Funkce, která zobrazí informační alert
  public open_info_alert(): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() =>{
      //Nastavení proměnné na log1
      this.is_info_icon_alert_activated = true;
    });
  }

  public get_preview_char_data(): CharTempsData {
    const data: CharTempsData | null = AppComponent.get_full_json_temp_char();
    return [...(data?.[0] ? [data[0]] : this.default_preview_graph_data)];
  }

  //Funkce, která zavře informační alert
  public close_info_alert(): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() =>{
      //Nastavení proměnné na log0
      this.is_info_icon_alert_activated = false;
    });
  }

  //Funkce, která vrátí vnitřní teplotu
  public get_in_temp(): string {
    return AppComponent.get_in_temp();
  }

  //Funkce, která vrátí venkovní teplotu
  public get_out_temp(): string {
    return AppComponent.get_out_temp();
  }

  //Funkce, která vrátí teplotu chladiče
  public get_cooler_temp(): string {
    return AppComponent.get_cooler_temp();
  }

  protected readonly open = open;
}
