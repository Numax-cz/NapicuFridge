import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {CharacteristicController} from "../../../CharacteristicController";

@Component({
  selector: 'app-settings',
  templateUrl: './settings.component.html',
  styleUrls: ['./settings.component.scss', '../../main.page.scss'],
})
export class SettingsComponent {

  public selected_item: number = 2;



  constructor(public ngZone: NgZone) {

  }



  public change_system_power(value: number): void {
    this.selected_item = value;
  }

  public change_in_fans(event: any): void {
    CharacteristicController.writeInFansAvailable(event.currentTarget.checked)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Přepsání proměnné v nastavení
        AppComponent.fridge_data.config.fridge_in_fans = event.currentTarget.checked;
      })
    })
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
