import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {CharacteristicController} from "../../../CharacteristicController";
import {alert_animations} from "../../Animation";


@Component({
  selector: 'app-settings',
  templateUrl: './settings.component.html',
  styleUrls: ['./settings.component.scss', '../../main.page.scss'],
  animations: alert_animations
})
export class SettingsComponent {

  //Proměnná určující, zda je menu pro přejmenování zařízení aktivní
  public rename_device_menu_activated: boolean = false;
  //Proměnná určující, zda input není validní
  public rename_input_on_error: boolean = false;
  //Proměnná ukládající aktuální hodnotu v inputu
  public rename_input_value: string = "NapicuFridge";

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

  //TODO Odpojit zařízení při přejmenování

  //Funkce pro změnu stavu vnitřních ventilátorů
  public change_in_fans(event: any): void {
    //Uložení event hodnoty do konstantní proměnné
    const value: boolean = event.currentTarget.checked;
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

  //Funkce, která otevře menu pro přejmenování zařízení
  public open_rename_device_menu(): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      this.rename_device_menu_activated = true;
    });
  }

  //Funkce, která zavře menu pro přejmenování zařízení
  public close_rename_device_menu(): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      this.rename_device_menu_activated = false;
    });
  }

  //Funkce pro změnu nastavení piezo při chybě
  public change_buzzing_on_error(event: any): void {
    //Uložení event hodnoty do konstantní proměnné
    const value: boolean = event.currentTarget.checked;
    //Zavolání funkce pro zapsání stavu vnitřních ventilátorů
    CharacteristicController.writeBuzzingOnError(value)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Přepsání proměnné v nastavení
        AppComponent.fridge_data.config.buzzing_on_error = value;
      });
    });
  }

  //Funkce pro přejmenování chytré ledničky (Pokud se vrátí null, zařízení není připojené)
  public rename_device(): void {
    //Pokud je zařízení připojené a hodnota inputu je validní provede se následující
    if(this.get_is_connected() && !this.rename_input_on_error) {
      //Spuštění funkce pro zavření menu pro přejmenování zařízení
      this.close_rename_device_menu();
      //Spuštění funkce pro přejmenování zařízení
      //Funkce trim odstraní mezery z obou konců stringu a vrátí nový string
      CharacteristicController.renameDevice(this.rename_input_value.trim())?.then(() => {
        //Spuštění funkce uvnitř zóny Angularu
        this.ngZone.run(() => {
          //Nastavíme nové jméno zařízení pokud je připojené zařízení uložené v proměnné
          AppComponent.connected_device!.name = this.rename_input_value;
          //Spuštění funkce pro uložení dat o spárovaném zařízení
          AppComponent.save_paired_device_to_storage();
        });
      });
    }
  }

  //Funkce, která zkontroluje validaci hodnoty v inputu
  public check_input_rename_device_name(): void {
    //TODO DOCUMENTATION + VAR
      this.rename_input_on_error =  !(/^(?![ ])[A-Za-z0-9-_. ]{1,248}$/.test(this.rename_input_value));
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

  //Funkce, která vrátí zda při chybě bude bzučet piezo
  public get_buzzing_on_error(): boolean {
    return AppComponent.fridge_data.config.buzzing_on_error;
  }

  //Statická funkce, která vrátí jméno připojeného zařízení
  public get_paired_device_name(): string {
    return AppComponent.get_paired_device_name();
  }
}
