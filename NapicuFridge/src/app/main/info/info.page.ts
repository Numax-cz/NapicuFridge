import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../app.component";
import {alert_animations} from "../Animation";

@Component({
  selector: 'app-info',
  templateUrl: './info.page.html',
  styleUrls: ['./info.page.scss', '../main.page.scss'],
  animations: alert_animations
})
export class InfoPage {
  //Proměnná, která určuje zda je informační alert zobrazen
  public is_info_icon_alert_activated: boolean = false;

  public preview_graph_data = [
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


  constructor(protected ngZone: NgZone) { }

  //Funkce, která zobrazí informační alert
  public open_info_alert(): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() =>{
      //Nastavení proměnné na log1
      this.is_info_icon_alert_activated = true;
    });
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
