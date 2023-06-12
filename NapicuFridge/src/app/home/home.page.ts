import {Component, NgZone} from '@angular/core';

import {BLE} from "@awesome-cordova-plugins/ble";
import {AppVersion} from "@awesome-cordova-plugins/app-version";
import {AppComponent} from "../app.component";
import {environment} from "../../environments/environment";
import {BluetoothScanData} from "../interface/Bluetooth";
import {Router} from "@angular/router";


@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage {

  //Deklarace proměnné pod podmínkout
  //Pokud je aplikace v režimu "vývoje", nastaví se vyplněné pole.
  //Pokud tak není, pole je prázdné
  public devices: BluetoothScanData[] = environment.production? [] : [
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

  //Proměnná, která uchovává stav připojování k zařízení
  public connecting: boolean = false;


  constructor(private router: Router, private ngZone: NgZone) {
    let i: string | null = AppComponent.application_settings.getItem("auto_connect_mac_address");
    if(i) {
      //this.connect(i);
    }
  }

  //Funkce pro automatické připojení k poslednímu spárovanému zařízení
  public auto_connect(): void {

  }

  //Funkce pro zahájení skenování NapicuFridge zařízení
  public scan(): void {

    //Nastavit stav skenování na log1
    this.scanning = true;
    //Zahájení skenování bluetooth zařízení s požadovaným UUID
    BLE.startScan(["6E400001-B5A3-F393-E0A9-E50E24DCCA9E"]).subscribe(
      {
        next: (data: BluetoothScanData) => {
          //Provést následující po nalezení nového zařízení

          //Vypsání hodnoty do vývojářské konzoly
          console.log("New device");
          //Spuštění funkce uvnitř zóny Angularu
          this.ngZone.run(() => {
            //Přidání nového zařízení do proměnné
            this.devices.push(data);
          })
        },
        error: (data) => {
          //Vypsání chyby do vývojářské konzole
          console.log(data);
        }
      }
    );



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

    this.connect(this.devices[device_id].id);
  }

  private connect(device_id: string): void {
    this.connecting = true;
    BLE.connect(device_id).subscribe({
      next: (data: BluetoothScanData) => {
        console.log("Connected");
        AppComponent.connected_device = data.id;
        this.router.navigateByUrl("main");
        AppComponent.application_settings.setItem("auto_connect_mac_address", device_id);
      },
      error: (data) => {
        console.log(data);
      }
    });
  }

  //Funkce, která vrátí verzi aplikace v požadovaném formátu
  public get_app_version(): string {
    return `${AppComponent.application_name} ${AppComponent.application_version_code}`;
  }

}
