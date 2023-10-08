import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../app.component";
import {alert_animations} from "../Animation";
import {FridgePowerMode} from "../../interface/Enums";
import {CharacteristicController} from "../../CharacteristicController";
import {DEFAULT_IN_FANS_ON_SWITCH, DEFAULT_POWER_MODE_ON_SWITCH} from "../../config/configuration";


@Component({
  selector: 'app-device',
  templateUrl: './device.page.html',
  styleUrls: ['./device.page.scss', '../main.page.scss'],
  animations: alert_animations,
})
export class DevicePage {

  //Proměnná pro uložení stavu alertu
  public active_alert: boolean = false;

  constructor(private ngZone: NgZone) { }

  public change_power_mode(event: any) {
    //Pokud je spínač zapnutý provede se následující
    if(event.currentTarget.checked) {
      //Pokud je předchozí hodnota nastavena na vypnuto provede se následující
      if(AppComponent.get_previous_power_mode() == FridgePowerMode.FRIDGE_OFF_POWER) {
        //Spuštění funkce uvnitř zóny Angularu
        this.ngZone.run(() => {
          //Zapíšeme výchozí hodnotu
          AppComponent.fridge_data.config.fridge_previous_power_mode = DEFAULT_POWER_MODE_ON_SWITCH;
        });
      }
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
        //Zapíšeme aktuální napájecí režim do proměnné ukládající předchozí napájecí režim
        AppComponent.fridge_data.config.fridge_previous_power_mode = AppComponent.get_power_mode();
        //Zapíšeme aktuální hodnotu vnitřních ventilátorů do předchozí hodnoty vnitřních ventilátorů
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
