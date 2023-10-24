import {Component, NgZone, OnInit} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {CharTempsData} from "../../../interface/CharData";
import {CharSettings} from "../../../interface/FridgeData";
import {ViewWillLeave} from "@ionic/angular";

@Component({
  selector: 'app-charts',
  templateUrl: './charts.component.html',
  styleUrls: ['./charts.component.scss'],
})
export class ChartsComponent implements ViewWillLeave{

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
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      AppComponent.switch_in_temp_display_char();
    });
  }

  //Funkce, která nastaví obrácenou bool hodnotu proměnné určující zobrazení křivky venkovní teploty na grafu
  public switch_out_temp_display_char(): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      AppComponent.switch_out_temp_display_char();
    });
  }

  //Funkce, která nastaví obrácenou bool hodnotu proměnné určující zobrazení křivky teploty na chladiči v grafu
  public switch_cooler_temp_display_char(): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      AppComponent.switch_cooler_temp_display_char();
    });
  }

  //Funkce, která vrátí nastavení grafu
  public get_char_settings(): CharSettings {
      return AppComponent.get_char_settings();
  }
}
