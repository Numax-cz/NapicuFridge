import {Component} from '@angular/core';

import {BLE} from "@awesome-cordova-plugins/ble";
import {AppVersion} from "@awesome-cordova-plugins/app-version";
import {AppComponent} from "../app.component";
import {environment} from "../../environments/environment";

interface data {
  name: string,
  id: string,
  advertising: ArrayBuffer,
  rssi: number,
}


@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage {

  //Deklarace proměnné pod podmínkout
  //Pokud je aplikace v režimu "vývoje", nastaví se vyplněné pole.
  //Pokud tak není, pole je prázdné
  public devices: data[] = environment.production? [] : [
    {
      name: "NapicuFridge",
      id: "test",
      rssi: 32,
      advertising: new ArrayBuffer(0)
    },
    {
      name: "NapicuFridge2",
      id: "test",
      rssi: 32,
      advertising: new ArrayBuffer(0)
    }
  ];

  //Proměnná, která uchovává stav skenování
  public scanning: boolean = false;


  constructor() {
  }

  //Funkce pro automatické připojení k poslednímu spárovanému zařízení
  public auto_connect(): void {

  }

  //Funkce pro zahájení skenování NapicuFridge zařízení
  public scan(): void {

    this.scanning = true;


    BLE.startScan(["6E400001-B5A3-F393-E0A9-E50E24DCCA9E"]).subscribe(
      {
        next: (data: data) => {
          this.devices.push(data);
        },
        error: (data) => {
          console.log(data);
        }

      }
    )


    //TODO END scan + services

  }

  //Funkce pro stopnutí skenování
  public stop_scan(): void {
    BLE.stopScan();
    this.scanning = false;
  }

  //Funkce, která se spustí po kliknutí na vyhledané zařízení
  public on_click_device(device_id: number): void {
    this.stop_scan();




  }

  //Funkce, která vrátí verzi aplikace v požadovaném formátu
  public get_app_version(): string {
    return `${AppComponent.application_name} ${AppComponent.application_version_code}`;
  }

}
