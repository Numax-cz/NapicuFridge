import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../app.component";
import {environment} from "../../environments/environment";
import {Router} from "@angular/router";
import {Configuration} from "../config/configuration";
import {
  AndroidGattTransportMode,
  BluetoothLE,
  BondedStatus,
  DeviceInfo,
  ScanStatus
} from "@awesome-cordova-plugins/bluetooth-le";
import {Config} from "@ionic/angular";
import {BLE} from "@awesome-cordova-plugins/ble";
import {add} from "ionicons/icons";
import {DevicePage} from "../main/device/device.page";

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

  //Proměnná která uchovává stav
  public loading: boolean = true;


  constructor(private router: Router, private ngZone: NgZone) {
    //Spuštění hlavní funkce
    this.init();
  }

  //Funkce, která se spustí při spuštění aplikace
  public init(): void {

    BluetoothLE.initialize({statusReceiver: true, request: true}).subscribe(async () => {

      await BluetoothLE.hasPermissionBtScan().then(async (status: {hasPermission: boolean}) => {
        if(!status.hasPermission) await BluetoothLE.requestPermissionBtScan(); //TODO Při nepovolení => povolte BT
      });

      await BluetoothLE.hasPermissionBtConnect().then(async (status: {hasPermission: boolean}) => {
        if(!status.hasPermission) await BluetoothLE.requestPermissionBtConnect(); //TODO Při nepovolení => povolte BT
      });


      //Získání adresy z lokálního uložiště
      let i: DeviceInfo | null = AppComponent.get_paired_device_data_from_storage();
      if(i) {
        //Vypsání hodnoty do vývojářské konzole
        console.log(i);
        //Připojit se k adrese, pokud je uložena v lokálním uložišti
        this.on_select_device(i.address);
      }
      else this.loading = false;


    });
  }


  //Funkce pro zahájení skenování NapicuFridge zařízení
  public scan(): void {

    //Nastavit stav skenování na log1
    this.scanning = true;

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
    this.on_select_device(this.devices[device_id].address);
  }

  //Funkce pro připojení se k zařízení s požadovanou adresou
  private on_select_device(address: string): void {
    //Zkontroluje stav párování
    BluetoothLE.isBonded({address: address}).then((state: BondedStatus) => {
      //Když zařízení bylo již spárované
      if(state.isBonded) {
        //Přesměrování uživatele
        this.redirect_user(address);
      } else {
        //Spuštění funkce pro párování zařízení
        this.bond(address);
      }
    }).catch(e => {
      //Vypsání hodnoty do vývojářské konzole
      console.log("error: " + JSON.stringify(e));
    });
  }

  //Funkce pro spárování zařízení
  public bond(address: string): void {
    //Nastavení proměnné pairing na log1
    this.loading = true;
    BluetoothLE.initialize({statusReceiver: true, request: true}).subscribe(async () => {
      BluetoothLE.bond({address: address}).subscribe((data: DeviceInfo) => {
        //Když se zařízení páruje
        if(data.status === "bonding") {

          //Vypsání hodnoty do vývojářské konzole
          console.log("Bonding...");
        }

        //Když je zařízení odparované
        if(data.status === "unbonded") {
          //Vypsání hodnoty do vývojářské konzole
          console.log("Unbonded");

          // //Nastavení proměnné pairing na log0
          // this.loading = false;
          // //Smazání adresy spárovaného zaířzení
          // AppComponent.application_settings.removeItem("auto_connect_mac_address");
        }

        //Když je zařízení úspěšně spárované
        if(data.status === "bonded") {
          //Vypsání hodnoty do vývojářské konzole
          console.log("Bonded");
          //Přesměrování uživatele
          this.redirect_user(address);
        }
      });
    });
  }

  //Přesměrování uživatele na hlavní část aplikace (/main/info)
  public redirect_user(address: string): void {
    //Připojení se na zařízení
    AppComponent.connect(address);

    //Přesměrování uživatele na URL /main/info
    this.router.navigateByUrl("main/info");
  }

  //Funkce, která vrací verzi aplikace v požadovaném formátu
  public get_app_version(): string {
    return `${AppComponent.application_name} ${AppComponent.application_version_code}`;
  }

}
