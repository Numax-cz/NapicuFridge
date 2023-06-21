import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../app.component";
import {environment} from "../../environments/environment";
import {Router} from "@angular/router";
import {Configuration} from "../config/configuration";
import {AndroidGattTransportMode, BluetoothLE, DeviceInfo, ScanStatus} from "@awesome-cordova-plugins/bluetooth-le";
import {Config} from "@ionic/angular";
import {BLE} from "@awesome-cordova-plugins/ble";

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage {

  //Deklarace proměnné pod podmínkout
  //Pokud je aplikace v režimu "vývoje", nastaví se vyplněné pole.
  //Pokud tak není, pole je prázdné
  public devices: ScanStatus[] = environment.production? [] : [
    {
      name: "NapicuFridge",
      address: "test",
      rssi: 32,
      advertisement: "",
      status: "connected"
    },
    {
      name: "NapicuFridge2",
      address: "test",
      rssi: 32,
      advertisement: "",
      status: "connected"
    }
  ];

  //Proměnná, která uchovává stav skenování
  public scanning: boolean = false;

  //Proměnná která uchovává stav párování
  public pairing: boolean = false;


  constructor(private router: Router, private ngZone: NgZone) {


    //Získání adresy z lokálního uložiště
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

    //Inicializace bluetooth low energy
    BluetoothLE.initialize({statusReceiver: true, request: true}).subscribe(() => {
      // Po inicializaci se provede následující
      // Zahájení skenování bluetooth zařízení s požadovaným UUID
      BluetoothLE.startScan({services: [Configuration.SERVICE_UUID]}).subscribe((data: ScanStatus) => {
        //Provést následující po nalezení nového zařízení

        //Kontrola zda se zařízení již nenachází v najitých zařízení v proměnné devices
        if(data.address && !this.devices.some((device: ScanStatus) => {
          return data.address === device.address;
        })) {
          //Vypsání hodnoty do vývojářské konzole
          console.log("New device");
          //Spuštění funkce uvnitř zóny Angularu
          this.ngZone.run(() => {
            //Přidání nového zařízení do proměnné devices
            this.devices.push(data);
          });
        }
      })

    });









    //TODO END scan + services

  }

  //Funkce pro stopnutí skenování
  public stop_scan(): void {
    BluetoothLE.stopScan();
    this.scanning = false;
  }

  //Funkce, která se spustí po kliknutí na vyhledané zařízení
  public on_click_device(device_id: number): void {
    //Spuštění funkce pro stopnutí skenování
    this.stop_scan();
    //Spuštění funkce pro připojení
    this.connect(this.devices[device_id].address);
  }

  //Funkce pro připojení se k zařízení s požadovanou adresou
  private connect(address: string): void {








    BluetoothLE.bond({address: address}).subscribe((data: DeviceInfo) => {
      //Když se zařízení páruje
      if(data.status === "bonding") {
        //Nastavení proměnné pairing na log1
        this.pairing = true;
        //Vypsání hodnoty do vývojářské konzole
        console.log("Bonding...");
      }

      //Když je zařízení odparované
      if(data.status === "unbonded") {
        console.log("Unbonded");
      }

      //Když je zařízení úspěšně spárované
      if(data.status === "bonded") {
        //Vypsání hodnoty do vývojářské konzole
        console.log("Bonded");

        //Uložení adresy spárovaného zařízení do proměnné
        AppComponent.connected_device = address;
        //Přesměrování uživatele na URL /main
        this.router.navigateByUrl("main");
        //Uložení adresy spárovaného zaířzení
        AppComponent.application_settings.setItem("auto_connect_mac_address", address);
      }
    });
  }

  public read (address: string): void {

    // BluetoothLE.connect({address}).subscribe(() => {
    //   BluetoothLE.isConnected({address}).then(() => {
    //     console.log("Connected");
    //   }).catch(e => {
    //     console.log('Read err: ' + JSON.stringify(e));
    //   })
    //
    //
    // }, e => {
    //   console.log('Read err: ' + JSON.stringify(e));
    //
    // });


  }



  //Funkce, která vrátí verzi aplikace v požadovaném formátu
  public get_app_version(): string {
    return `${AppComponent.application_name} ${AppComponent.application_version_code}`;
  }

}
