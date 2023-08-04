import {Component, NgZone} from '@angular/core';
import {AnimationController, Platform} from "@ionic/angular";
import {StatusBar, Style} from "@capacitor/status-bar";
import {AppVersion} from "@awesome-cordova-plugins/app-version";
import {App} from "@capacitor/app";
import {
  AndroidGattTransportMode,
  BluetoothLE,
  Device,
  DeviceInfo,
  OperationResult
} from "@awesome-cordova-plugins/bluetooth-le";
import {FridgeData} from "./interface/FridgeData";
import {Configuration} from "./config/configuration";
import {app_animation} from "./main/Animation";
import {FridgeDisplayState} from "./interface/Enums";
import CHARACTERISTIC_UUID_TX = Configuration.CHARACTERISTIC_UUID_TX;
import {environment} from "../environments/environment";



@Component({
  selector: 'app-root',
  templateUrl: 'app.component.html',
  styleUrls: ['app.component.scss'],
})
export class AppComponent {


  //Funkce pro získání vlastní animace
  public get_animation = () => app_animation;

  //Statická proměnná pro local storage nastavení aplikace
  public static application_settings: Storage = window.localStorage;

  //Statická proměnná pro uložení jména aplikace
  public static application_name: string | null = null;

  //Statická proměnná pro uložení verze aplikace
  public static application_version_code: string | null = null;

  //Statická proměnná pro uložení základní informací připojeného zařízení (Pokud je aplikace spuštěna v testovacím režimu, nastaví se hodnoty pro vývoj aplikace)
  public static connected_device: DeviceInfo | null = environment.production? null : {name: "TestDevice", address: "TestAddress", status: "connected"};

  //Statická proměnná pro uložení NgZone (jedná se o službu pro provádění funkcí uvnitř zóny Anguláru)
  private static ngZone: NgZone;

  //Statická proměnná pro ukládání informací z ESP32
  public static fridge_data: FridgeData = {
    //Vnitřní teplota ledničky
    in_temp: "",


    config: {
      fridge_display_available: true,
      fridge_display_state: FridgeDisplayState.FRIDGE_DISPLAY_IN_TEMP_1
    }
  }

  constructor(private platform: Platform, ngZone: NgZone, private animationCtrl: AnimationController) {
    AppComponent.ngZone = ngZone;

    //Tento blok kódu nastaví, aby zpětné tlačítko vždy ukončilo aplikaci
    platform.backButton.subscribeWithPriority(-1, () => {
      //Ukončit aplikaci
      App.exitApp();
    });

    platform.ready().then(() => {
      //Kontrola zda je zařízení typu android
      if (platform.is('android')) {
        //Nastavení barvy textu horního status baru v mobilní aplikaci
        StatusBar.setStyle({style: Style.Light});
        //Nastavení, zda má stavový řádek překrývat webové zobrazení, aby bylo možné využít prostor pod ním.
        StatusBar.setOverlaysWebView({overlay: true});
        //Získání jména aplikace
        AppVersion.getAppName().then((value: string) => AppComponent.application_name = value);
        //Získání verze aplikace
        AppVersion.getVersionNumber().then((value: string) => AppComponent.application_version_code = value);

        //Zamknutí orientace aplikace (na výšku)
        screen.orientation.lock("portrait");
      }
    });
  }

  //Funkce pro připojení se k zařízení
  public static connect(address: string): void {
    //Kontrola, zda je zařízení spárované
    if(!this.connected_device) {
      //Připojit se k zařízení
      BluetoothLE.connect({address: address, transport: AndroidGattTransportMode.TRANSPORT_LE, autoConnect: true}).subscribe((device: DeviceInfo) =>  {
        if(device.status === "connected") {
          //Uložení adresy spárovaného zaířzení
          AppComponent.application_settings.setItem("device", JSON.stringify(device));

          //Po úspěšném připojení provést následující
          //Nastavit proměnnou pro připojené zařízení
          this.set_connected_device(device);
          //Vypsání hodnoty do vývojářské konzole
          console.log("connected");


          //Tato funkce zjistí, zda byly zjištěny charakteristiky a deskriptory zařízení,
          //nebo zda došlo k chybě, pokud nebylo inicializováno nebo není připojeno k zařízení.
          BluetoothLE.discover({address: address, clearCache: true})
            .then((d: Device) => {
              //Synchronizování nastavení na ESP32
              this.update_config_from_esp();
              //Přihlášení se k odběru charakteristiky vnitřní teploty
              this.subscribe_in_temp();
            }).catch((e) =>{
            //Vypsání hodnoty do vývojářské konzole
            console.log("error_discovered" + JSON.stringify(e));
          });
        }
        else if (device.status === "disconnected") {
          //Po odpojení provést následující
          //Nastavit proměnnou connected_device na null
          this.set_connected_device();
          console.log("Disconnected from device: " + device.address, "status");
        }
      });
    }
  }

  //Funkce, která synchronizuje nastavení, které je aktuálně nastavené na ESP32
  public static update_config_from_esp(): void {
    if(this.connected_device) {



      BluetoothLE.read({address: this.connected_device.address, service: Configuration.SERVICE_UUID, characteristic: Configuration.CHARACTERISTIC_DISPLAY_ENABLE_UUID})
        .then((data: OperationResult) => {
          //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
          let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
          //Převést bytes na string
          let value: string = BluetoothLE.bytesToString(bytes);

          this.fridge_data.config.fridge_display_available = (value == "1");
      }).catch((e) => {
        console.log("error_discovered" + JSON.stringify(e));

      })
    }
  }

  //Funkce, která nastaví hodnotu proměnné connected_device. Bez udání parametru je hodnota nastavená na null => zařízení není připojené
  private static set_connected_device(value: DeviceInfo | null = null): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      //Nastavení proměnné dle parametru (value výchozí: null)
      this.connected_device = value;
    });
  }

  //Funkce pro přihlášení se k odběru pro získávání dat z vnitřního teploměru
  private static subscribe_in_temp(): void {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device) {
      //Přihlášení se k odběru charakteristiky vnitřní teploty
      BluetoothLE.subscribe({
        address: AppComponent.connected_device.address,
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
                this.fridge_data.in_temp = value;
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

  //Funkce, která obnoví tovární nastavení
  public static factory_reset(): void {

  }

  //Funkce, která vrátí uložená data o spárovaném zařízení
  public static get_paired_device_data_from_storage(): DeviceInfo | null {
    let i: string | null = AppComponent.application_settings.getItem("device");
    if(i) return JSON.parse(i) as DeviceInfo;
    return null;
  }

  //Funkce, která vrátí jméno připojeného zařízení
  public static get_paired_device_name(): string {
    return this.get_paired_device_data_from_storage()?.name || "";
  }

  //Funkce, která vrátí adresu MAC připojeného zařízení
  public static get_paired_device_address(): string {
    return this.get_paired_device_data_from_storage()?.address || "";
  }

  //Funkce, která vrátí zda je zařízení připojené
  public static is_connected(): boolean {
    return !!(this.connected_device);
  }

  //Funkce, která vrátí vnitřní teplotu
  public static get_in_temp(): string {
    return this.fridge_data.in_temp;
  }

  //Funkce, která vrátí zda je displej chytré ledničky povolen
  public static get_is_display_available(): boolean {
    return AppComponent.fridge_data.config.fridge_display_available;
  }

  //Funkce, která vrátí nastavený stav displeje
  public static get_display_state(): FridgeDisplayState {
    return AppComponent.fridge_data.config.fridge_display_state;
  }
}
