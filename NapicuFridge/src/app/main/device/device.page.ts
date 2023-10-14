import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../app.component";
import {alert_animations, show_box_animations} from "../Animation";
import {FridgePowerMode} from "../../interface/Enums";
import {CharacteristicController} from "../../CharacteristicController";
import {DEFAULT_IN_FANS_ON_SWITCH, DEFAULT_POWER_MODE_ON_SWITCH} from "../../config/configuration";


@Component({
  selector: 'app-device',
  templateUrl: './device.page.html',
  styleUrls: ['./device.page.scss', '../main.page.scss'],
  animations: [alert_animations, show_box_animations],
})
export class DevicePage {

  public active_error_info_in_temp: boolean = false;

  //Proměnná pro uložení stavu alertu
  public active_alert: boolean = false;

  constructor(private ngZone: NgZone) { }

  public change_power_mode(event: any) {
    //Pokud je spínač zapnutý provede se následující
    if(event.currentTarget.checked) {
      //Spustíme funkci pro zápis charakteristiky na předchozí nastavený napájecí režim
      CharacteristicController.writePowerMode(AppComponent.get_previous_power_mode())?.then(() => {
        //Až se úspěšně provede zápis charakteristiky provede se následující
        //Spuštění funkce uvnitř zóny Angularu
        this.ngZone.run(() => {
          //Zapíšeme aktuální režim napájení
          AppComponent.fridge_data.config.fridge_power_mode = AppComponent.get_previous_power_mode();
        });
      });

      //Zavolání funkce pro zapsání stavu vnitřních ventilátorů
      CharacteristicController.writeInFansAvailable(AppComponent.get_is_previous_in_fans_enabled())?.then(() => {
        //Až se úspěšně provede zápis charakteristiky provede se následující
        //Spuštění funkce uvnitř zóny Angularu
        this.ngZone.run(() => {
          //Přepsání proměnné v nastavení
          AppComponent.fridge_data.config.fridge_in_fans = AppComponent.get_is_previous_in_fans_enabled();
        });
      });
      return;
    }

    //Přepneme napájecí režim
    CharacteristicController.writePowerMode(FridgePowerMode.FRIDGE_OFF_POWER)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Spuštění funkce pro uložení aktuálního napájecího režimu
        AppComponent.set_previous_power_mode(AppComponent.get_power_mode());
        //Spuštění funkce pro uložení nastavení vnitřních ventilátorů
        AppComponent.set_previous_in_fans(AppComponent.get_is_in_fans_enabled());
        //Zapíšeme vypnutý režim do proměnné
        AppComponent.fridge_data.config.fridge_power_mode = FridgePowerMode.FRIDGE_OFF_POWER;
      });
    });
  }

  //Funkce, která se spustí po kliknutí na tlačítko "Obnovit tovární nastavení"
  public on_click_factory_reset(): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      //Nastavení proměnné na log1
      this.active_alert = true;
    });
  }

  //Funkce, která vrátí zda došlo v ledničce k problému
  public get_is_fridge_on_error(): boolean {
    return AppComponent.get_is_fridge_on_error();
  }

  //Funkce, která vrátí zda došlo v ledničce k vážné poruše
  public get_is_fridge_on_fatal_error(): boolean {
    return AppComponent.get_is_fridge_on_fatal_error();
  }

  //Tato funkce obnoví tovární nastavení. Funkce se spustí po kliknutí na tlačítko
  public factory_reset(): void {
    AppComponent.factory_reset();
  }

  //Funkce, která vrátí zda je zařízení připojené
  public is_connected(): boolean {
    return AppComponent.is_connected();
  }

  //Funkce, která vrátí jméno připojeného zařízení
  public get_paired_device_name(): string {
    return AppComponent.get_paired_device_name();
  }

  //Funkce, která vrátí režim napájení ledničky
  public get_power_mode(): FridgePowerMode {
    return AppComponent.get_power_mode();
  }

  //Funkce, která vrátí hodnotu vypnutého režimu
  public get_fridge_off_mode(): number {
    return FridgePowerMode.FRIDGE_OFF_POWER;
  }
}
