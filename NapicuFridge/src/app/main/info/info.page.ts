import {Component} from '@angular/core';
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

  constructor() { }

  public open_info_alert(): void {

  }

  public close_info_alert(): void {
    
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
}
