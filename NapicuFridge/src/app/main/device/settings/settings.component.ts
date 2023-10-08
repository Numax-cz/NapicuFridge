import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {CharacteristicController} from "../../../CharacteristicController";

@Component({
  selector: 'app-settings',
  templateUrl: './settings.component.html',
  styleUrls: ['./settings.component.scss', '../../main.page.scss'],
})
export class SettingsComponent {

  //Proměnná ukládající nastavený režim
  public declare selected_item: number;

  constructor(public ngZone: NgZone) {
    //Nastavíme proměnnou na stav napájení
    this.selected_item = AppComponent.fridge_data.config.fridge_power_mode;
  }

  //Funkce pro změnu režimu napájení
  public change_power_mode(value: number): void {
    //Zavolání funkce pro zapsání režimu napájení
    CharacteristicController.writePowerMode(value)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Přepsání proměnné
        this.selected_item = value;
        //Zapíšeme stav napájení do proměnné ukládající aktuální stav napájení
        AppComponent.fridge_data.config.fridge_power_mode = value;
        //Zapíšeme stav napájení do proměnné ukládající předchozí stav napájení
        AppComponent.fridge_data.config.fridge_previous_power_mode = value;
      });
    });
  }

  //Funkce pro změnu stavu vnitřních ventilátorů
  public change_in_fans(event: any): void {
    //Uložení event hodnoty do konstantní proměnné
    const value = event.currentTarget.checked;
    //Zavolání funkce pro zapsání stavu vnitřních ventilátorů
    CharacteristicController.writeInFansAvailable(value)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Přepsání proměnné v nastavení
        AppComponent.fridge_data.config.fridge_in_fans = value;
      });
    });
  }

  //Funkce, která vrátí zda je zařízení připojené
  public get_is_connected(): boolean {
    return AppComponent.is_connected();
  }

  //Funkce, která vrátí zda jsou vnitřní ventilátory zapnuté
  public get_is_in_fans_enabled(): boolean {
    return AppComponent.get_is_in_fans_enabled();
  }
}
