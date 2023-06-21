import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../app.component";
import {Configuration} from "../config/configuration";
import {
  AndroidGattTransportMode,
  BluetoothLE,
  Device,
  DeviceInfo,
  OperationResult
} from "@awesome-cordova-plugins/bluetooth-le";
import CHARACTERISTIC_UUID_TX = Configuration.CHARACTERISTIC_UUID_TX;


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
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device) {
      //Proměnná pro uložení adresy
      let address: string = AppComponent.connected_device;
      //Připojit se k zařízení
      BluetoothLE.connect({address: address, transport: AndroidGattTransportMode.TRANSPORT_LE}).subscribe(() =>  {
        //Po úspěšném připojení provést nasledující
        //Vypsání hodnoty do vývojářské konzole
        console.log("connected");
          //Tato funkce zjistí, zda byly zjištěny charakteristiky a deskriptory zařízení,
          //nebo zda došlo k chybě, pokud nebylo inicializováno nebo není připojeno k zařízení.
          BluetoothLE.discover({address: address, clearCache: true})
            .then((d: Device) => {
              //Přihlášení se k odběru charakteristiky vnitřní teploty
              this.subscribe_in_temp();
            }).catch((e) =>{
            //Vypsání hodnoty do vývojářské konzole
            console.log("error_discovered" + JSON.stringify(e));
          });
      });
    }
  }


  //Funkce pro přihlášení se k odběru pro získávání dat z vnitřního teploměru
  public subscribe_in_temp(): void {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device) {
      //Přihlášení se k odběru charakteristiky vnitřní teploty
      BluetoothLE.subscribe({
        address: AppComponent.connected_device,
        service: Configuration.SERVICE_UUID,
        characteristic: CHARACTERISTIC_UUID_TX
      }).subscribe(
        {
          next: (data: OperationResult) => {
            //Po získání dat z bluetooth charakteristiky provést následující
            if(data.value) {
              //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
              let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
              //Převést bytes na string
              let value: string = BluetoothLE.bytesToString(bytes);
              //Spuštění funkce uvnitř zóny Angularu
              this.ngZone.run(() => {
              //Zapsat převedený bytes na string do proměnné in_temp
                this.data.in_temp = value;
              })
            }
          },
          error: (e) => {
            //Vypsání hodnoty do vývojářské konzole
            console.log("error" + JSON.stringify(e));
          }
        }
      );
    }
  }




  public on_click(): void {

  }


}
