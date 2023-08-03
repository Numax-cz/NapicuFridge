import {Component, NgZone, OnInit} from '@angular/core';
import {FridgeDisplayState} from "../../../interface/Enums";
import {AppComponent} from "../../../app.component";
import {animate, state, style, transition, trigger} from "@angular/animations";
import {BluetoothLE} from "@awesome-cordova-plugins/bluetooth-le";
import {Configuration} from "../../../config/configuration";
import CHARACTERISTIC_DISPLAY_ENABLE_UUID = Configuration.CHARACTERISTIC_DISPLAY_ENABLE_UUID;

@Component({
  selector: 'app-display',
  templateUrl: './display.component.html',
  styleUrls: ['./display.component.scss', '../../main.page.scss'],
  animations: [

    //Programování animací skrze Angular
    trigger('ContentAnimation', [
      state('void', style({ opacity: 0, transform: 'translateY(50%)' })),
      transition(':enter', [
        animate(150, style({ opacity: 1, transform: 'translateY(0%)' }))
      ]),
      transition(':leave', [
        animate(150, style({ opacity: 0, transform: 'translateY(50%)' }))
      ]),
    ]),

  ],
})
export class DisplayComponent {

  constructor(public ngZone: NgZone) { }



  //TODO synchronizace
  //Funkce, která se zavolá po změně přepínače pro vypáníní displeje
  public display_available_input_change(event: any): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      //Nastavení proměnné z configu na novou hodnotu
      AppComponent.fridge_data.config.fridge_display_available = event.currentTarget.checked;
    });
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device)  {
      //Převedení stringu do bytes
      let bytes: Uint8Array = BluetoothLE.stringToBytes(AppComponent.fridge_data.config.fridge_display_available ? "1" : "0");
      //Funkce pro převod pole unit8Array na řetězec v kódování base64 pro zápis znaků nebo deskriptorů
      let encodedUnicodeString: string = BluetoothLE.bytesToEncodedString(bytes);
      //Napsání charakteristiky
      BluetoothLE.write({
        address: AppComponent.connected_device.address,
        service: Configuration.SERVICE_UUID,
        characteristic: CHARACTERISTIC_DISPLAY_ENABLE_UUID,
        value: encodedUnicodeString,
      });
    }
  }


  //Funkce, která vrátí zda je displej chytré ledničky povolen
  public get_is_display_available(): boolean {
    return AppComponent.get_is_display_available();
  }







}
