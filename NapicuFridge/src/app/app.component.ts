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
import {alert_animations, app_animation} from "./main/Animation";
import {FridgeDisplayState, FridgePowerMode} from "./interface/Enums";
import {environment} from "../environments/environment";
import {CharacteristicController} from "./CharacteristicController";
import {DEFAULT_IN_FANS_ON_SWITCH, DEFAULT_POWER_MODE_ON_SWITCH} from "./config/configuration";

@Component({
  selector: 'app-root',
  templateUrl: 'app.component.html',
  styleUrls: ['app.component.scss'],
  animations: alert_animations
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

  //Statická proměnná pro uložení hodnoty, zda je alert viditelný
  public static device_connection_alert_display: boolean = false;

  //Statická proměnná pro ukládání informací z ESP32
  public static fridge_data: FridgeData = {
    //Vnitřní teplota ledničky
    in_temp: "",
    out_temp: "",
    cooler_temp: "",
    config: {
      fridge_display_available: true,
      fridge_display_state: FridgeDisplayState.FRIDGE_DISPLAY_IN_TEMP_1,
      fridge_in_fans: false,
      buzzing_on_error: true,
      fridge_power_mode: FridgePowerMode.FRIDGE_OFF_POWER,
    },
    errors: {
      fridge_out_temp: false,
      fridge_in_temp: false,
      fridge_cooler_temp: false
    }
  }

  //Statická proměnná, která určuje zda došlo v ledničce k vážné poruše
  public static fridge_fatal_error: boolean = false;

  constructor(private platform: Platform, ngZone: NgZone, private animationCtrl: AnimationController) {
    //Nastavení ngZone na statickou proměnnou
    AppComponent.ngZone = ngZone;

    //Tento blok kódu nastaví, aby zpětné tlačítko vždy ukončilo aplikaci
    platform.backButton.subscribeWithPriority(-1, () => {
      //Ukončit aplikaci
      App.exitApp();
    });

    //Deklarace funkce, která se spustí po přípravě platformy
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

  //Funkce, která zobrazí a následně skryje alert pro zobrazování stavu připojení
  public static show_connection_alert(): void {
    //Nastavení proměnné na log1
    this.device_connection_alert_display = true;
    //Spuštění funkce pro vykonaní funkce po 3500ms
    setTimeout(() => {
      //Nastavení proměnné na log0
      this.device_connection_alert_display = false;
    }, 3500);
  }

  //Statická funkce pro připojení se k zařízení
  private static async connect(address: string): Promise<void> {
    return new Promise<void>((resolve, reject) => {
      //Kontrola, zda je zařízení spárované
      if(!this.connected_device) {
        //Připojit se k zařízení
        BluetoothLE.connect({address: address, transport: AndroidGattTransportMode.TRANSPORT_LE}).subscribe({
          next: async (device: DeviceInfo) => {
            //Spuštění funkce po připojení zařízení
            await this.on_next_connect(device);
            //Spuštění resolve funkce Promisu
            resolve();
          },
          error: (e: any) => {
            //Spuštění reject funkce Promisu
            reject();
          }
        });
      } else resolve();
    })
  }

  //Statická funkce po připojení zařízení
  private static async on_next_connect(device: DeviceInfo): Promise<void> {
    return new Promise<void>((resolve, reject) => {
      if(device.status === "connected") {
        //Uložení adresy spárovaného zaířzení
        AppComponent.application_settings.setItem("device", JSON.stringify(device));

        //Po úspěšném připojení provést následující
        //Nastavit proměnnou pro připojené zařízení
        AppComponent.set_connected_device(device);
        //Vypsání hodnoty do vývojářské konzole
        console.log("connected");

        //Tato funkce zjistí, zda byly zjištěny charakteristiky a deskriptory zařízení,
        //nebo zda došlo k chybě, pokud nebylo inicializováno nebo není připojeno k zařízení.
        BluetoothLE.discover({address: device.address, clearCache: true})
          .then(async (d: Device) => {
            //Synchronizování nastavení na ESP32
            await AppComponent.update_config_from_esp();
            //Přihlášení se k odběru charakteristiky vnitřní teploty
            await AppComponent.subscribe_in_temp();
            //Přihlášení se k odběru charakteristiky venkovní teploty
            await AppComponent.subscribe_out_temp();
            //Přihlášení se k odběru charakteristiky teploty chladiče
            await AppComponent.subscribe_cooler_temp();
            //Spuštění resolve funkce Promisu
            resolve();
          }).catch((e) => {
          //Vypsání hodnoty do vývojářské konzole
          console.error("error_discovered" + JSON.stringify(e));
          //Spuštění reject funkce Promisu
          reject();
        });
      }
      else if (device.status === "disconnected") {
        //Po odpojení provést následující
        //Nastavit proměnnou connected_device na null
        AppComponent.set_connected_device();
        //Vypsání hodnoty do vývojářské konzole
        console.log("Disconnected from device: " + device.address, "status");
        //Spuštění funkce pro automatické připojení k zařízení
        AppComponent.start_auto_connect(device.address);
        //Spuštění resolve funkce Promisu
        resolve();
      }
    });
  }

  //Asynchronní funkce, která se snaží o automatické připojení k zařízení
  public static async start_auto_connect(address: string): Promise<void> {
    //Vypsání hodnoty do vývojářské konzole
    console.log("Auto Connecting...");
    //Funkce pro zavření/vyřazení zařízení Bluetooth LE
    await BluetoothLE.close({address: address}).catch(() => {});
    //Následující blok se snaží o znovuřipojení se k zařízení
    try {
      //Spuštění asynchronní funkce pro znovu se připojení k zařízení
      await AppComponent.connect(address);
      //Vypsání hodnoty do vývojářské konzole
      console.log("Reconnection was successful");
    } catch (error) {
      //Pokud se nepodaří o znovupřipojení provede se následující
      //Vypsání hodnoty do vývojářské konzole
      console.error("Error when reconnecting");
    }
    //Podmínka, pokud je zařízení připojené
    if(AppComponent.connected_device?.address) return;
    //Znovu spuštění funkce po 1s (1000ms)
    setTimeout(() => this.start_auto_connect(address), 1000);
  }

  //Statická funkce, která synchronizuje nastavení, které je aktuálně nastavené na ESP32
  public static async update_config_from_esp(): Promise<void> {
    //Získání zda je displej povolen
    await CharacteristicController.readIsDisplayAvailable()
      ?.then((data: OperationResult) => {
        //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
        let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
        //Převést bytes na string
        let value: string = BluetoothLE.bytesToString(bytes);
        //Nastavení proměnné na hodnotu podle získaných dat
        this.fridge_data.config.fridge_display_available = (value == "1");
      }).catch((e) => {
        //Vypsání hodnoty do vývojářské konzole
        console.error("error_discovered" + JSON.stringify(e));
      });

    //Získání stavu displeje
    await CharacteristicController.readDisplayState()
      ?.then((data: OperationResult) => {
        //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
        let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
        //Převést bytes na string
        let value: string = BluetoothLE.bytesToString(bytes);
        //Převedení string na number a následné nastavení proměnné na hodnotu získaných dat
        this.fridge_data.config.fridge_display_state = Number(value);
      }).catch((e) => {
      //Vypsání hodnoty do vývojářské konzole
      console.error("error_discovered" + JSON.stringify(e));
    });

    //Získání zda jsou vnitřní ventilátory povolené
    await CharacteristicController.readInFansAvailable()
      ?.then((data: OperationResult) => {
        //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
        let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
        //Převést bytes na string
        let value: string = BluetoothLE.bytesToString(bytes);
        //Nastavení proměnné na hodnotu podle získaných dat
        this.fridge_data.config.fridge_in_fans = (value == "1");
      }).catch((e) => {
      //Vypsání hodnoty do vývojářské konzole
      console.error("error_discovered" + JSON.stringify(e));
    });

    //Získání režim napájení
    await CharacteristicController.readPowerMode()
      ?.then((data: OperationResult) => {
        //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
        let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
        //Převést bytes na string
        let value: string = BluetoothLE.bytesToString(bytes);
        //Převedení string na number a následné nastavení proměnné na hodnotu získaných dat
        this.fridge_data.config.fridge_power_mode = Number(value);
        //Pokud není výchozí hodnota na stav vypnuto provede se následující
        if (this.fridge_data.config.fridge_power_mode != FridgePowerMode.FRIDGE_OFF_POWER) {
          //Spuštění funkce pro uložení předchozího režimu  ledničky
          this.set_previous_power_mode(this.fridge_data.config.fridge_power_mode);
        }
      }).catch((e) => {
      //Vypsání hodnoty do vývojářské konzole
      console.error("error_discovered" + JSON.stringify(e));
    });

    //Získání režimu piezo při chybě
    await CharacteristicController.readBuzzingOnError()
      ?.then((data: OperationResult) => {
        //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
        let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
        //Převést bytes na string
        let value: string = BluetoothLE.bytesToString(bytes);
        //Nastavení proměnné na hodnotu podle získaných dat
        this.fridge_data.config.buzzing_on_error = (value == "1");
      }).catch((e) => {
        //Vypsání hodnoty do vývojářské konzole
        console.error("error_discovered" + JSON.stringify(e));
    });
  }

  //Statická funkce, která nastaví hodnotu proměnné connected_device. Bez udání parametru je hodnota nastavená na null => zařízení není připojené
  private static set_connected_device(value: DeviceInfo | null = null): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      //Nastavení proměnné dle parametru (value výchozí: null)
      this.connected_device = value;
      //Zobrazení informačního alertu
      AppComponent.show_connection_alert();
    });
  }

  //Statická funkce pro přihlášení se k odběru pro získávání dat z vnitřního teploměru
  private static subscribe_in_temp(): Promise<void> {
    return new Promise<void>((resolve, reject) => {
      //Spuštění funkce pro přihlášení se k odběru charakteristiky vnitřní teploty
      CharacteristicController.subscribeInTemp()?.subscribe(
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
                //Pokud získaná hodnota je rovna "nan" provede se následující
                //Zapíšeme do proměnné o vnitřní chybě log1
                if(value === "nan") this.fridge_data.errors.fridge_in_temp = true;
                //Pokud získaná hodnota není rovna "nan" provede se následující
                else {
                  //Zapsat převedený bytes na string do proměnné in_temp
                  this.fridge_data.in_temp = value;
                  //Zapíšeme do proměnné o vnitřní chybě teploměru log0
                  this.fridge_data.errors.fridge_in_temp = false;
                }
              });
              //Spuštění resolve funkce Promisu
              resolve();
            }
          },
          error: (e) => {
            //Vypsání hodnoty do vývojářské konzole
            console.log("error" + JSON.stringify(e));
            //Spuštění reject funkce Promisu
            reject();
          }
        }
      );
    })
  }

  //Statická funkce pro přihlášení se k odběru pro získávání dat z venkovního teploměru
  private static subscribe_out_temp(): Promise<void> {
    return new Promise((resolve, reject) => {
      //Přihlášení se k odběru charakteristiky venkovní teploty
      CharacteristicController.subscribeOutTemp()?.subscribe(
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
                //Pokud získaná hodnota je rovna "nan" provede se následující
                //Zapíšeme do proměnné o venkovní chybě teploměru log1
                if(value === "nan") this.fridge_data.errors.fridge_out_temp = true;
                //Pokud získaná hodnota není rovna "nan" provede se následující
                else {
                  //Zapsat převedený bytes na string do proměnné out_temp
                  this.fridge_data.out_temp = value;
                  //Zapíšeme do proměnné o venkovní chybě teploměru log0
                  this.fridge_data.errors.fridge_in_temp = false;
                }
              });
              //Spuštění resolve funkce Promisu
              resolve();
            }
          },
          error: (e) => {
            //Vypsání hodnoty do vývojářské konzole
            console.log("error" + JSON.stringify(e));
            //Spuštění reject funkce Promisu
            reject();
          }
        }
      );
    });
  }

  //Statická funkce pro přihlášení se k odběru pro získávání dat z teploměru na chladiči
  private static subscribe_cooler_temp(): Promise<void> {
    return new Promise((resolve, reject) => {
      //Přihlášení se k odběru charakteristiky teploty chladiče
      CharacteristicController.subscribeCoolerTemp()?.subscribe(
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
                //Pokud získaná hodnota je rovna "nan" provede se následující
                //Zapíšeme do proměnné o chybě teploměru na chladiči log1
                if(value === "nan") this.fridge_data.errors.fridge_cooler_temp = true;
                //Pokud získaná hodnota není rovna "nan" provede se následující
                else {
                  //Zapsat převedený bytes na string do proměnné cooler_temp
                  this.fridge_data.cooler_temp = value;
                  //Zapíšeme do proměnné o chybě teploměru na chladiči log0
                  this.fridge_data.errors.fridge_cooler_temp = false;
                }
              });
              //Spuštění resolve funkce Promisu
              resolve();
            }
          },
          error: (e) => {
            //Vypsání hodnoty do vývojářské konzole
            console.log("error" + JSON.stringify(e));
            //Spuštění reject funkce Promisu
            reject();
          }
        }
      );
    });
  }

  //Funkce, která vrátí aktuální hodnotu na daném displej statu
  public static get_display_value_by_state(): string | null {
    //TODO DOC
    switch (AppComponent.fridge_data.config.fridge_display_state) {
      case FridgeDisplayState.FRIDGE_DISPLAY_IN_TEMP_1:
        return AppComponent.fridge_data.in_temp;
      case FridgeDisplayState.FRIDGE_DISPLAY_OUT_TEMP_1:
        return AppComponent.fridge_data.out_temp;
      default:
        return null;
    }
  }

  //Statická funkce, která obnoví tovární nastavení
  public static factory_reset(): void {
    //TODO přidat komunikaci s ESP32
  }

  //Statická funkce, která vrátí uložená data o spárovaném zařízení
  public static get_paired_device_data_from_storage(): DeviceInfo | null {
    //Získání uložených dat
    let i: string | null = AppComponent.application_settings.getItem("device");
    //Pokud existuje uložená hodnota provede se následující
    if(i) return JSON.parse(i) as DeviceInfo;
    //Vrácení null pokud uložená data neexistují
    return null;
  }

  //Statická funkce, která vrátí jméno připojeného zařízení
  public static get_paired_device_name(): string {
    return this.get_paired_device_data_from_storage()?.name || "";
  }

  //Statická funkce, která vrátí adresu MAC připojeného zařízení
  public static get_paired_device_address(): string {
    return this.get_paired_device_data_from_storage()?.address || "";
  }

  //Statická funkce, která vrátí zda je zařízení připojené
  public static is_connected(): boolean {
    return !!(this.connected_device);
  }

  //Statická funkce, která vrátí vnitřní teplotu
  public static get_in_temp(): string {
    return this.fridge_data.in_temp;
  }

  //Statická funkce, která vrátí venkovní teplotu
  public static get_out_temp(): string {
    return this.fridge_data.out_temp;
  }

  //Statická funkce, která vrátí teplotu chladiče
  public static get_cooler_temp(): string {
    return this.fridge_data.cooler_temp;
  }

  //Statická funkce, která vrátí zda je displej chytré ledničky povolen
  public static get_is_display_available(): boolean {
    return AppComponent.fridge_data.config.fridge_display_available;
  }

  //Statická funkce, která vrátí nastavený stav displeje
  public static get_display_state(): FridgeDisplayState {
    return AppComponent.fridge_data.config.fridge_display_state;
  }

  //Funkce, která vrátí zda je zařízení připojené
  public get_is_connected(): boolean {
      return AppComponent.is_connected();
  }

  //Funkce, která vrátí zda je alert zobrazen
  public get_device_connection_alert_display(): boolean {
    return AppComponent.device_connection_alert_display;
  }

  //Statická funkce, která vrátí zda jsou vnitřní ventilátory zapnuté
  public static get_is_in_fans_enabled(): boolean {
    return AppComponent.fridge_data.config.fridge_in_fans;
  }

  //Statická funkce, která nastaví předchozí nastavení vnitřních ventilátorů
  public static set_previous_in_fans(value: boolean): void {
    //Uložení nastavení
    AppComponent.application_settings.setItem("previous_in_fans", JSON.stringify(value));
  }

  //Statická funkce, která vrátí zda jsou vnitřní ventilátory zapnuté
  public static get_is_previous_in_fans_enabled(): boolean {
    //Získání uložených dat
    let i: string | null = AppComponent.application_settings.getItem("previous_in_fans");
    //Pokud existuje uložená hodnota provede se následující
    if(i) return JSON.parse(i) as boolean;
    //Nastavení výchozí hodnoty
    this.set_previous_in_fans(DEFAULT_IN_FANS_ON_SWITCH);
    //Vrácení výchozí hodnoty
    return DEFAULT_IN_FANS_ON_SWITCH;
  }

  //Statická funkce, která vrátí režim napájení ledničky
  public static get_power_mode(): FridgePowerMode {
    return AppComponent.fridge_data.config.fridge_power_mode;
  }

  //Statická funkce, která nastaví předchozí napájecí režim ledničky
  public static set_previous_power_mode(value: FridgePowerMode): void {
    //Uložení nastavení
    AppComponent.application_settings.setItem("previous_power_mode", JSON.stringify(value));
  }

  //Statická funkce, která vrátí předchozí režim napájení ledničky
  public static get_previous_power_mode(): FridgePowerMode {
    //Získání uložených dat
    let i: string | null = AppComponent.application_settings.getItem("previous_power_mode");
    //Pokud existuje uložená hodnota provede se následující
    if(i) return JSON.parse(i) as FridgePowerMode;
    //Nastavení výchozí hodnoty
    this.set_previous_power_mode(DEFAULT_POWER_MODE_ON_SWITCH);
    //Vrácení výchozí hodnoty
    return DEFAULT_POWER_MODE_ON_SWITCH;
  }

  //Statická funkce, která vrátí zda při chybě bude bzučet piezo
  public static get_buzzing_on_error(): boolean {
    return this.fridge_data.config.buzzing_on_error;
  }

  //Statická funkce, která vrátí zda došlo v ledničce k problému
  public static get_is_fridge_on_error(): boolean {
    return this.fridge_data.errors.fridge_in_temp
      || this.fridge_data.errors.fridge_out_temp
      || this.fridge_data.errors.fridge_cooler_temp;
  }

  //Statická funkce, která vrátí zda je vnitřní teploměr v chybě
  public static get_is_in_temp_in_error(): boolean {
    return this.fridge_data.errors.fridge_in_temp;
  }

  //Statická funkce, která vrátí zda je venkovní teploměr v chybě
  public static get_is_out_temp_in_error(): boolean {
    return this.fridge_data.errors.fridge_out_temp;
  }

  //Statická funkce, která vártí zda je teploměr na chladiči v chybě
  public static get_is_cooler_temp_in_error(): boolean {
    return this.fridge_data.errors.fridge_cooler_temp;
  }

  //Statická funkce, která vrátí zda došlo v ledničce k vážné poruše
  public static get_is_fridge_on_fatal_error(): boolean {
    return this.fridge_fatal_error;
  }
}
