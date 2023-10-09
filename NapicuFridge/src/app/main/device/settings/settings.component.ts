import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {CharacteristicController} from "../../../CharacteristicController";

@Component({
  selector: 'app-settings',
  templateUrl: './settings.component.html',
  styleUrls: ['./settings.component.scss', '../../main.page.scss'],
})
export class SettingsComponent {

  constructor(public ngZone: NgZone) { }

  //Funkce pro změnu režimu napájení
  public change_power_mode(value: number): void {
    if(!this.get_is_connected() || this.get_power_mode() === 0) return;
    //Zavolání funkce pro zapsání režimu napájení
    CharacteristicController.writePowerMode(value)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Zapíšeme stav napájení do proměnné ukládající aktuální stav napájení
        AppComponent.fridge_data.config.fridge_power_mode = value;
        //Zapíšeme stav napájení do proměnné ukládající předchozí stav napájení
        AppComponent.set_previous_power_mode(value);
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

  //Funkce, která vrátí zda došlo v ledničce k vážné poruše
  public get_is_fridge_on_fatal_error(): boolean {
    return AppComponent.get_is_fridge_on_fatal_error();
  }

  //Funkce, která vrátí zda jsou vnitřní ventilátory zapnuté
  public get_is_in_fans_enabled(): boolean {
    //Podmínka pokud není zařízení připojené nastaví se předchozí hodnota, pokud je, nastaví se aktuální nastavená hodnota
    return (!this.get_is_connected()) ? AppComponent.get_is_previous_in_fans_enabled() : AppComponent.get_is_in_fans_enabled();
  }

  //Funkce, která vrátí který napájecí režim je zapnutý
  public get_selected_power_mode(): number {
    //Podmínka pokud není zařízení připojené nastaví se předchozí hodnota, pokud je, nastaví se aktuální nastavená hodnota
    return (!this.get_is_connected() || AppComponent.get_power_mode() == 0) ? AppComponent.get_previous_power_mode() : AppComponent.get_power_mode();
  }

  //Funkce, která vrátí režim napájení ledničky
  public get_power_mode(): number {
    return AppComponent.get_power_mode();
  }
}
