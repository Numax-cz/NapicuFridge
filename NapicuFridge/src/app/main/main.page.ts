import {Component, NgZone} from '@angular/core';
import {BLE} from "@awesome-cordova-plugins/ble";
import {AppComponent} from "../app.component";
import {Configuration} from "../config/configuration";


interface data {
  in_temp: string
}

@Component({
  selector: 'app-main',
  templateUrl: './main.page.html',
  styleUrls: ['./main.page.scss'],
})
export class MainPage  {

  public data: data = {
    in_temp: ""
  }

  constructor(private ngZone: NgZone) {
    //Kontrola, zda je připojené zařízení
    if(AppComponent.connected_device) {
      //Získání aktuálních informací o teplotě
      BLE.startNotification(AppComponent.connected_device, Configuration.SERVICE_UUID, Configuration.CHARACTERISTIC_UUID_TX).subscribe(
        {
          next: (buffer) => {
            //Vytvoření DataView proměnné
            let dv = new DataView(buffer[0]);
            //Spuštění funkce uvnitř zóny Angularu
            this.ngZone.run(() => {
              //Nastavení získané hodnoty z bluetooth do proměnné
              this.data.in_temp = new TextDecoder().decode(dv.buffer);
            });
            //Vypsání hodnoty do vývojářské konzoly
            console.log(`New in_temp: ${this.data.in_temp}`);
          }
        }
      )
    }
  }




  public on_click(): void {


    var data = new Uint8Array(1);

    data[0] = 1;

    if(AppComponent.connected_device) {
      BLE.write(AppComponent.connected_device, "6E400001-B5A3-F393-E0A9-E50E24DCCA9E", "6E400002-B5A3-F393-E0A9-E50E24DCCA9E", data.buffer);
    }
  }


}
