import {Component, NgZone, OnDestroy, OnInit} from '@angular/core';
import {AppComponent} from "../../../app.component";
import {CharacteristicController} from "../../../CharacteristicController";
import {BluetoothLE, OperationResult} from "@awesome-cordova-plugins/bluetooth-le";
import {ViewDidLeave} from "@ionic/angular";

@Component({
  selector: 'app-information',
  templateUrl: './information.component.html',
  styleUrls: ['./information.component.scss', "../../main.page.scss"],
})
export class InformationComponent implements ViewDidLeave {

  protected uptime: number = 0;

  protected interval_id: number | undefined = undefined;

  constructor(public ngZone: NgZone) {
      this.interval_id = setInterval(this.read_uptime, 1000);
  }

  ionViewDidLeave(): void {
    clearInterval(this.interval_id);
  }

  public read_uptime = (): void => {
    console.log("update");
    CharacteristicController.readUptime()?.then((data: OperationResult) => {
      //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
      let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
      //Převést bytes na string
      let value: string = BluetoothLE.bytesToString(bytes);
      //Spuštění funkce uvnitř zóny Angularu
      this.ngZone.run(() => {
        //Převedeme string na číslo a následně nastavíme proměnnou uptime
        this.uptime = Number(value);
      });
    });
  }

  //Funkce, která vrátí jméno připojeného zařízení
  public get_paired_device_name(): string {
    return AppComponent.get_paired_device_name();
  }

  //Funkce, která vrátí adresu MAC připojeného zařízení
  public get_paired_device_address(): string {
    return AppComponent.get_paired_device_address();
  }

  //Funkce, která vrátí dobu od spuštění chytré ledničky
  public get_uptime(): string {
    //Konstantní proměnná pro uložení sekund
    const seconds = Math.floor(this.uptime / 1000); //Převod milisekund na sekundy
    //Konstantní proměnná pro uložení hodin
    const hours = Math.floor(seconds / 3600); //Výpočet hodin
    //Konstantní proměnná pro uložení minut
    const minutes = Math.floor((seconds % 3600) / 60); //Výpočet minut
    //Konstantní proměnná pro uložení zbývajicích sekund
    const remainingSeconds = seconds % 60; //Výpočet zbývajících sekund
    //Vrácení formátovaného výsledku
    return`${hours}:${minutes}:${remainingSeconds}`;
  }

}
