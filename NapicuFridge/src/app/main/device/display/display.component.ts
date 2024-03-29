import {Component, NgZone, OnInit} from '@angular/core';
import {FridgeDisplayState} from "../../../interface/Enums";
import {AppComponent} from "../../../app.component";
import {animate, state, style, transition, trigger} from "@angular/animations";
import {BluetoothLE} from "@awesome-cordova-plugins/bluetooth-le";
import {Configuration} from "../../../config/configuration";
import {CharacteristicController} from "../../../CharacteristicController";
import {CharTempsData} from "../../../interface/CharData";


@Component({
  selector: 'app-display',
  templateUrl: './display.component.html',
  styleUrls: ['./display.component.scss', '../../main.page.scss'],
  animations: [
    // //Programování animací skrze Angular
    // trigger('ContentAnimation', [
    //   state('void', style({ opacity: 0, transform: 'translateY(50%)' })),
    //   transition(':enter', [
    //     animate(150, style({ opacity: 1, transform: 'translateY(0%)' }))
    //   ]),
    //   transition(':leave', [
    //     animate(150, style({ opacity: 0, transform: 'translateY(50%)' }))
    //   ]),
    // ]),
  ],
})
export class DisplayComponent {


  public declare selected_item: number;

  constructor(public ngZone: NgZone) {
    this.selected_item = AppComponent.fridge_data.config.fridge_display_state;
  }

  //Funkce, která vrátí hodnotu, která se má zobrazovat na chytré ledničce
  public get_display_value(): string {
    return AppComponent.get_display_value_by_state() || "NULL";
  }

  //Funkce pro změnu stavu displeje
  public change_input_display_state(value: number): void {
    //Pokud zařízení není připojené, nebo display není povolen provede se následující
    if(!this.get_is_connected() || !this.get_is_display_available()) return;
    //Zavolání funkce pro zapsání stavu displeje
    CharacteristicController.writeDisplayState(value)?.then(() => {
      //Až se úspěšně provede zápis charakteristiky provede se následující
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Přepsání proměnné
        this.selected_item = value;
        //Přepsání proměnné v nastavení
        AppComponent.fridge_data.config.fridge_display_state = value;
      });
    }).catch((e) => {
      //Vypsání hodnoty do vývojářské konzole
      console.error("error_write" + JSON.stringify(e));
    });
  }

  //Funkce, která se zavolá po změně přepínače pro vypínání/zapínání displeje
  public display_available_input_change(event: any): void {
   //Proměnná, která uchovává aktuální stav přepínače
    const slider: boolean = event.currentTarget.checked;
    //Zavolání funkce pro vypnutí, nebo zapnutí displeje
    CharacteristicController.writeDisplayAvailable(slider)?.then(() => {
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Nastavení proměnné z configu na novou hodnotu
        AppComponent.fridge_data.config.fridge_display_available = slider;
      });
    });
  }

  //Funkce, která vrátí zda došlo v ledničce k vážné poruše
  public get_is_fridge_on_fatal_error(): boolean {
    return AppComponent.get_is_fridge_on_fatal_error();
  }

  //Funkce, která vrátí zda je displej chytré ledničky povolen
  public get_is_display_available(): boolean {
    return AppComponent.get_is_display_available();
  }

 //Funkce, která vrátí zda je zařízení připojené
  public get_is_connected(): boolean {
    return AppComponent.is_connected();
  }
}
