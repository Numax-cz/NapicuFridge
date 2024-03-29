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
import {CharSettings, FridgeData, FridgeJSONData} from "./interface/FridgeData";
import {alert_animations, app_animation} from "./main/Animation";
import {FridgeDisplayState, FridgePowerMode} from "./interface/Enums";
import {environment} from "../environments/environment";
import {CharacteristicController} from "./CharacteristicController";
import {
  CHAR_COOLER_TEMP_TEXT,
  CHAR_DEFAULT_VIEW_RESOLUTION_INDEX,
  CHAR_IN_TEMP_TEXT,
  CHAR_LAST_UPDATE_DATE_FORMAT,
  CHAR_OUT_TEMP_TEXT,
  CHAR_VIEW_RESOLUTION_OPTIONS,
  DEFAULT_ALERT_DISPLAY_TIME,
  DEFAULT_CHAR_VIEW_DATA_FOR_DEV,
  DEFAULT_CHARTS_HINT,
  DEFAULT_DELETE_COLOR_HINT,
  DEFAULT_FAVOURITES_COLOURS_LED,
  DEFAULT_IN_FANS_ON_SWITCH,
  DEFAULT_POWER_MODE_ON_SWITCH
} from "./config/configuration";
import {CharTempsData} from "./interface/CharData";
import {NapicuDate} from "napicuformatter";
import {CopyArray} from "./main/CopyArray";
import {Clipboard} from '@capacitor/clipboard';
import {NapicuOptionsData} from "./interface/NapicuOption";
import {RGB, RGBA} from "ngx-color/helpers/color.interfaces";

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
  public static connected_device: DeviceInfo | null = environment.production ? null : {name: "TestDevice", address: "TestAddress", status: "connected"};

  //Statická proměnná pro uložení NgZone (jedná se o službu pro provádění funkcí uvnitř zóny Anguláru)
  private static ngZone: NgZone;

  //Statická proměnná pro uložení hodnoty, zda je alert viditelný
  public static device_connection_alert_display: boolean = false;

  //Statická proměnná pro uložení hodnoty, zda je alert o úspěšném resetování zařízení viditelný
  public static device_factory_reset_alert_display: boolean = false;

  //Statická proměnná pro uložení jednotlivých balíčků naměřených hodnot
  protected static temp_json_graph_string: string | null = null;

  //Statická proměnná pro uložení času ve kterém se naposledy aktualizoval graf naměřených hodnot
  protected static json_graph_last_update_date: number = 0;

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
      fridge_stop_on_open_door: true,
      fridge_led_enable: true,
      fridge_led_rgb: {r: 255, g: 255, b: 255, a: 255},
      fridge_led_brightness: -1,
    },
    errors: {
      fridge_out_temp: false,
      fridge_in_temp: false,
      fridge_cooler_temp: false,
      fridge_fan: false
    },
    char_settings: {
      display_in_temp: true,
      display_out_temp: true,
      display_cooler_temp: true,
      display_resolution: CHAR_DEFAULT_VIEW_RESOLUTION_INDEX
    },
    user_favorites_colors: DEFAULT_FAVOURITES_COLOURS_LED,
    user_delete_color_hint: DEFAULT_DELETE_COLOR_HINT,
    user_char_hint: DEFAULT_CHARTS_HINT,
    json_graph_chars_format: environment.production ? null : DEFAULT_CHAR_VIEW_DATA_FOR_DEV,
    json_graph_chars_format_view: null,
    json_graph_resolution_view: []
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
      //Spuštění funkce pro načtení uložených hodnot v zařízení
      AppComponent.load_config_from_storage();
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
        // @ts-ignore
        screen.orientation.lock("portrait"); //Jelikož tato funkce není v některých prohlížečích již dostupná, použijeme ts-ignore
        //Pokud není aplikace ve vývojářském režimu provede se následující
        if(!environment.production) AppComponent.update_char_view_data();
      }
    });
  }

  //Funkce, která zobrazí a následně skryje alert pro zobrazování stavu připojení
  public static show_connection_alert(): void {
    //Nastavení proměnné na log1
    this.device_connection_alert_display = true;
    //Spuštění funkce pro vykonaní funkce po dobu definovanou proměnnou DEFAULT_ALERT_DISPLAY_TIME
    setTimeout(() => {
      //Nastavení proměnné na log0
      this.device_connection_alert_display = false;
    }, DEFAULT_ALERT_DISPLAY_TIME);
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
            //Spuštění funkce pro přihlášení se k odběru charakteristiky vnitřní teploty
            AppComponent.subscribe_in_temp();
            //Spuštění funkce pro přihlášení se k odběru charakteristiky venkovní teploty
            AppComponent.subscribe_out_temp();
            //Spuštění funkce pro přihlášení se k odběru charakteristiky teploty chladiče
            AppComponent.subscribe_cooler_temp();
            //Spuštění funkce pro přihlášení se k odběru charakteristiky JSON dat
            AppComponent.subscribe_json_data();
            //Spuštění funkce pro přihlášení se k odběru charakteristiky napájecího režimu
            AppComponent.subscribe_power_mode();
            //Spuštění funkce pro přihlášení se k odběru charakteristiky
            AppComponent.subscribe_error_state();
            //Spuštění funkce pro vynucení naměřených JSON dat z chytré ledničky
            AppComponent.force_json_data();
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

    //Získání stavu, zda se má lednička pozastavit při otevřených dveří
    await CharacteristicController.readFridgeStopOnDoorOpen()
      ?.then((data: OperationResult) => {
        //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
        let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
        //Převést bytes na string
        let value: string = BluetoothLE.bytesToString(bytes);
        //Nastavení proměnné na hodnotu podle získaných dat
        this.fridge_data.config.fridge_stop_on_open_door = (value == "1");
      }).catch((e) => {
        //Vypsání hodnoty do vývojářské konzole
        console.error("error_discovered" + JSON.stringify(e));
      });

    //Získání stavu, zda se má LED osvětlení zapnout při otevřených dveří
    await CharacteristicController.readLEDEnable()
      ?.then((data: OperationResult) => {
        //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
        let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
        //Převést bytes na string
        let value: string = BluetoothLE.bytesToString(bytes);
        //Nastavení proměnné na hodnotu podle získaných dat
        this.fridge_data.config.fridge_led_enable = (value == "1");
      }).catch((e) => {
        //Vypsání hodnoty do vývojářské konzole
        console.error("error_discovered" + JSON.stringify(e));
      });

    //Získání nastavené barvy LED osvětlení
    await CharacteristicController.readLEDColor()
      ?.then((data: OperationResult) => {
        //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
        let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
        //Převést bytes na string
        let value: string = BluetoothLE.bytesToString(bytes);

        // Rozdělení řetězce podle čárky a odstranění bílých znaků
        const values: number[] = value.split(',').map(d => parseInt(d.trim(), 10));

        // Ověření, zda jsou k dispozici tři hodnoty
        if (values.length === 3 && values.every(d => !isNaN(d))) {
          //Získání hodnoty červené barvy ze stringu
          let R: number = values[0];
          //Získání hodnoty zelené barvy ze stringu
          let G: number = values[1];
          //Získání hodnoty modré barvy ze stringu
          let B: number = values[2];

          //Nastavení proměnné na hodnotu podle získaných dat
          this.fridge_data.config.fridge_led_rgb = {
            r: R,
            g: G,
            b: B,
            a: 255
          }
        }
    });

    //Získání hodnoty jasu LED osvětlení
    await CharacteristicController.readLEDBrightness()
      ?.then((data: OperationResult) => {
        //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
        let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
        //Převést bytes na string
        let value: string = BluetoothLE.bytesToString(bytes);
        //Nastavení proměnné na hodnotu podle získaných dat
        this.fridge_data.config.fridge_led_brightness = Number(value);
      });

    //Získání stavu chyb
    await CharacteristicController.readErrorState()
      ?.then((data: OperationResult) => {
      //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
      let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
      //Převést bytes na string
      let value: string = BluetoothLE.bytesToString(bytes);
      //Spuštění funkce pro nastavení chyb
      this.setErrorsByLog(value);
    });
  }

  //Statická funkce, která načte uložené hodnoty
  public static load_config_from_storage(): void {
    //Uložení a získání naměřených hodnot v json formátu pro graf
    this.fridge_data.json_graph_chars_format = this.get_json_temp_char_from_storage();
    //Uložení a získání nastavení grafu
    this.fridge_data.char_settings = this.get_char_settings_from_storage();
    //Uložení a získání uložených barev v seznamu oblíbených z uložiště
    this.fridge_data.user_favorites_colors = this.get_user_favorites_colors_from_storage();
    //Uložení a získání informací o nápovědě
    this.fridge_data.user_delete_color_hint = this.get_is_delete_color_hint_enabled_from_storage();
    //Uložení a získání informací o nápovědě ke grafu
    this.fridge_data.user_char_hint = this.get_is_charts_hint_enabled_from_storage();
    //Uložení a získání posledního času aktualizování grafu
    this.json_graph_last_update_date = this.get_json_graph_last_update_time_from_storage();
  }

  //Statická funkce, která nastaví hodnotu proměnné connected_device. Bez udání parametru je hodnota nastavená na null => zařízení není připojené
  private static set_connected_device(value: DeviceInfo | null = null): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      //Nastavení proměnné dle parametru (value výchozí: null)
      this.connected_device = value;
      //Zobrazení informačního alertu
      AppComponent.show_connection_alert();

      //Spuštění funkce pro uložení adresy spárovaného zaířzení
      this.save_paired_device_to_storage();
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
                //Pokud získaná hodnota není rovna "nan" provede se následující
                if(value !== "nan") {
                  //Zapsat převedený bytes na string do proměnné in_temp
                  this.fridge_data.in_temp = value;
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
                //Pokud získaná hodnota není rovna "nan" provede se následující
                if(value !== "nan") {
                  //Zapsat převedený bytes na string do proměnné out_temp
                  this.fridge_data.out_temp = value;
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
                //Pokud získaná hodnota není rovna "nan" provede se následující
                if (value !== "nan"){
                  //Zapsat převedený bytes na string do proměnné cooler_temp
                  this.fridge_data.cooler_temp = value;
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

  //Statická funkce pro přihlášení se k odběru pro získávání naměřených JSON dat z chytré ledničky
  private static subscribe_json_data(): Promise<void> {
      return new Promise((resolve, reject) => {
          //Přihlášení se k odběru charakteristiky JSON dat
          CharacteristicController.subscribeJsonData()?.subscribe(
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
                            //Pokud se získaná hodnota rovná "#START" provede se následující
                            if(value == "#START") {
                              this.temp_json_graph_string = "";
                            }
                            //Pokud se získaná hodnota rovná "#END" provede se následující
                            else if(value == "#END") {
                              this.fridge_data.json_graph_chars_format = this.format_json_to_char();
                              //Pokud jsou data v proměnné pro ukládaní dat k zobrazení v grafu
                              if(this.fridge_data.json_graph_chars_format_view) {
                                //Spuštění funkce pro aktualizovaní dat, které se mají zobrazit v grafu
                                this.update_char_view_data();
                              }
                              //Spuštění funkce pro uložení grafu
                              this.save_json_temp_char_to_storage(this.fridge_data.json_graph_chars_format);
                              //Uložení času pro zjištění času poslední aktualizace grafu
                              this.json_graph_last_update_date = new Date().getTime();
                              //Spuštění funkce pro uložení čas poslední aktualizace grafu naměřených hodnot
                              this.save_json_graph_last_update_date();
                              //Nastaví se proměnná na prázdný string
                              this.temp_json_graph_string = null;
                            } else {
                              //Přidáme získanou hodnotu do proměnné
                              this.temp_json_graph_string += value;
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
          )
      });
  }

  //Statická funkce pro přihlášení se k odběru pro získávání změň napájecího režimu z chytré ledničky
  private static subscribe_power_mode(): Promise<void> {
    return new Promise((resolve, reject) => {
      CharacteristicController.subscribePowerMode()?.subscribe(
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
                //Převedení string na number a následné nastavení proměnné na hodnotu získaných dat
                this.fridge_data.config.fridge_power_mode = Number(value[0]);
                //Převedení string na number následně na boolean hodnotu a následné nastavení proměnné na hodnotu získaných dat
                this.fridge_data.config.fridge_in_fans = Boolean(Number(value[1]));
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

  //Statická funkce pro přihlášení se k odběru pro získávání stavů errorů
  public static subscribe_error_state(): Promise<void> {
    return new Promise((resolve, reject) => {
      CharacteristicController.subscribeErrorState()?.subscribe(
        {
          next: (data: OperationResult) => {
            //Po získání dat z bluetooth charakteristiky provést následující
            if(data.value) {
              //Převést string v kódování base64 z hodnoty charakteristiky na objekt uint8Array
              let bytes: Uint8Array = BluetoothLE.encodedStringToBytes(data.value);
              //Převést bytes na string
              let value: string = BluetoothLE.bytesToString(bytes);
              //Spuštění funkce pro nastavení chyb
              this.setErrorsByLog(value);
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

  //Statická funkce, která nastaví proměnné chyb podle vstupních dat
  private static setErrorsByLog(error_log: string): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      //Převedení string na number následně na boolean hodnotu a následné nastavení proměnné na hodnotu získaných dat
      this.fridge_data.errors.fridge_in_temp = !Boolean(Number(error_log[0]));
      //Převedení string na number následně na boolean hodnotu a následné nastavení proměnné na hodnotu získaných dat
      this.fridge_data.errors.fridge_out_temp = !Boolean(Number(error_log[1]));
      //Převedení string na number následně na boolean hodnotu a následné nastavení proměnné na hodnotu získaných dat
      this.fridge_data.errors.fridge_cooler_temp = !Boolean(Number(error_log[2]));
      //Převedení string na number následně na boolean hodnotu a následné nastavení proměnné na hodnotu získaných dat
      this.fridge_data.errors.fridge_fan = !Boolean(Number(error_log[3]));
      //Pokud platí následující podmínka, nastaví se proměnná, která určuje kritické chyby na log1
      if(this.fridge_data.errors.fridge_cooler_temp || this.fridge_data.errors.fridge_fan) this.fridge_fatal_error = true;
    });
  }

  //Statická funkce pro vynucení naměřených JSON dat z chytré ledničky (Pokud se vrátí null, zařízení není připojené)
  private static force_json_data(): void {
    //Spuštění funkce pro vynucení naměřených JSON dat z chytré ledničky
    CharacteristicController.forceJSONData();
  }

  //Funkce, která vrátí aktuální hodnotu na daném displej statu
  public static get_display_value_by_state(): string | null {
    //V následující části vrátíme hodnotu, která se má zobrazit na displeji, podle proměnné
    switch (AppComponent.fridge_data.config.fridge_display_state) {
      case FridgeDisplayState.FRIDGE_DISPLAY_IN_TEMP_1:
        return AppComponent.fridge_data.in_temp;
      case FridgeDisplayState.FRIDGE_DISPLAY_OUT_TEMP_1:
        return AppComponent.fridge_data.out_temp;
      case FridgeDisplayState.FRIDGE_DISPLAY_COOLER_TEMP:
        return AppComponent.fridge_data.cooler_temp
      default:
        return null;
    }
  }

  //Statická funkce, která formátuje json do formátu json pro graf
  protected static format_json_to_char(): CharTempsData {
    //Převedení stringu na formát JSON
    let json: FridgeJSONData = JSON.parse(this.temp_json_graph_string || "");
    //Převedení pole uchovavající venkovní teplotu do formátu pro vytvoření grafu
    // (Pokud se nějaká hodnota v objektu po převedení ze stringu na number rovná NaN, nastaví se nula)
    let out_temp_series_char_format: {value: number, name: string}[] =
      json.out_temp.map((value: number, index: number) => {return {value: isNaN(value) ? 0 : value, name: `${index + 1}m`}});
    //Převedení pole uchovavající vnitřní teplotu do formátu pro vytvoření grafu
    // (Pokud se nějaká hodnota v objektu po převedení ze stringu na number rovná NaN, nastaví se nula)
    let in_temp_series_char_format: {value: number, name: string}[] =
      json.in_temp.map((value: number, index: number) => {return {value: isNaN(value) ? 0 : value, name: `${index + 1}m`}});
    //Převedení pole uchovavající teplotu chladiče do formátu pro vytvoření grafu
    // (Pokud se nějaká hodnota v objektu po převedení ze stringu na number rovná NaN, nastaví se nula)
    let cooler_temp_series_char_format: {value: number, name: string}[] =
      json.cooler_temp.map((value: number, index: number) => {return {value: isNaN(value) ? 0 : value, name: `${index + 1}m`}});
    //Vrácení zformátovaných dat do objektu
    return [
      {name: CHAR_OUT_TEMP_TEXT, series: out_temp_series_char_format},
      {name: CHAR_IN_TEMP_TEXT, series: in_temp_series_char_format},
      {name: CHAR_COOLER_TEMP_TEXT, series: cooler_temp_series_char_format},
    ];
  }

  //Statická funkce, která uloží json graf naměřených teplot
  protected static save_json_temp_char_to_storage(json: CharTempsData): void {
    //Uložení grafu
    AppComponent.application_settings.setItem("temp_char", JSON.stringify(json));
  }

  //Statická funkce, která vrátí uložený json graf naměřených teplot
  protected static get_json_temp_char_from_storage(): CharTempsData | null{
    //Získání uložených dat
    let i: string | null = AppComponent.application_settings.getItem("temp_char");
    //Pokud existuje uložená hodnota provede se následující
    if(i) return JSON.parse(i) as CharTempsData;
    //Vrácení výchozích hodnot pokud uložená data neexistují
    return AppComponent.fridge_data.json_graph_chars_format;
  }

  //Statická funkce, která vrátí celý json graf naměřených teplot
  public static get_full_json_temp_char(): CharTempsData | null {
    return this.fridge_data.json_graph_chars_format;
  }

  //Statická funkce, která vrátí uložené nastavení grafů
  protected static get_char_settings_from_storage(): CharSettings {
      //Získání uložených dat
      let i: string | null = AppComponent.application_settings.getItem("char_settings");
      //Pokud existuje uložená hodnota provede se následující
      if(i) return JSON.parse(i) as CharSettings;
      //Vrácení výchozích hodnot pokud uložená data neexistují
      return this.fridge_data.char_settings;
  }

  //Statická funkce, která vrátí uložený čas poslední aktualizace grafu naměřených hodnot
  protected static get_json_graph_last_update_time_from_storage(): number {
    //Získání uložených dat
    let i: string | null = AppComponent.application_settings.getItem("char_last_update");
    //Pokud existuje uložená hodnota provede se následující
    if(i) return parseInt(i);
    //Vrácení výchozích hodnot pokud uložená data neexistují
    return 0;
  }

  //Statická funkce, která uloží čas poslední aktualizace grafu naměřených hodnot
  protected static save_json_graph_last_update_date(): void {
    AppComponent.application_settings.setItem("char_last_update", this.json_graph_last_update_date.toString());
  }

  //Statická funkce, která uloží aktuální nastavení grafů
  public static save_char_settings(): void {
    AppComponent.application_settings.setItem("char_settings", JSON.stringify(this.fridge_data.char_settings));
  }

  //Statická funkce, která vrátí nastavení grafu
  public static get_char_settings(): CharSettings {
      return this.fridge_data.char_settings;
  }

  //Statický funkce pro uvedení chytré ledničky do továrního nastavení
  public static factory_reset(): void {
    //Uložení do proměnné log1
    AppComponent.device_factory_reset_alert_display = true;

    CharacteristicController.factoryRestart()?.then(() => {
        //TODO FUNKCE PRO ZOBRAZENÍ
    });
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

  //Statická funkce, která uloží data o spárovaném zařízení
  public static save_paired_device_to_storage(): void {
    //Pokud je připojené zařízení uložené v proměnné provede se následující
    if(this.connected_device) {
      //Uložení adresy spárovaného zařízení
      AppComponent.application_settings.setItem("device", JSON.stringify(this.connected_device));
    }
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

  //Funkce, která vrátí zda se má alert o úspěšném resetu zařízení zobrazovat
  public get_device_factory_reset_alert_display(): boolean {
    return AppComponent.device_factory_reset_alert_display;
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

  //Statická funkce, která vrátí zda je systém ledničky pozastaven
  public static get_is_fridge_paused(): boolean {
    return AppComponent.get_power_mode() == FridgePowerMode.FRIDGE_PAUSED;
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
    if(i && (JSON.parse(i) as FridgePowerMode) !== FridgePowerMode.FRIDGE_OFF_POWER) return JSON.parse(i) as FridgePowerMode;
    //Nastavení výchozí hodnoty
    this.set_previous_power_mode(DEFAULT_POWER_MODE_ON_SWITCH);
    //Vrácení výchozí hodnoty
    return DEFAULT_POWER_MODE_ON_SWITCH;
  }

  //Statická funkce, která aktualizuje data naměřených teplot, které se mají zobrazit v grafu
  public static update_char_view_data(): void {
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      this.fridge_data.json_graph_chars_format_view = CopyArray(this.get_full_json_temp_char()?.filter((value) => {
        return (
          (value.name === CHAR_IN_TEMP_TEXT && this.get_char_settings().display_in_temp) ||
          (value.name === CHAR_OUT_TEMP_TEXT && this.get_char_settings().display_out_temp) ||
          (value.name === CHAR_COOLER_TEMP_TEXT && this.get_char_settings().display_cooler_temp));
      })) || null;

      /////////////////////////////////////////////////////////////////////
      //V následujícím bloku kódu omezíme délky naměřených dat na
      // maximální délku definovanou proměnnou CHAR_MAX_DATA_VIEW_LENGTH
      //Spuštění funkce uvnitř zóny Angularu

     // Pokud existují data, provede se následující úpravy
      if (this.fridge_data.json_graph_chars_format_view) {
        //Provedeme for loop všech dat v proměnné obsahující veškeré data, které se mají zobrazit v grafu
        for (let i = 0; i < this.fridge_data.json_graph_chars_format_view.length; i++) {
          //Pokud je délka naměřených dat větší, nebo rovno hodnotě v objektu CHAR_VIEW_RESOLUTION_OPTIONS na indexu CHAR_DEFAULT_VIEW_RESOLUTION_INDEX
          if (this.fridge_data.json_graph_chars_format_view[i].series.length  > this.get_char_resolution()) {
            // Oříznutí objektu naměřených dat na maximální délku
            this.fridge_data.json_graph_chars_format_view[i].series = this.fridge_data.json_graph_chars_format_view[i].series.slice(-this.get_char_resolution() - 1);
          }
        }
      }
      // /////////////////////////////////////////////////////////////////////

      //Spuštění funkce pro aktualizaci dostupných rozlišení grafu
      this.update_char_available_resolutions();
    });
  }

  //Statická funkce, která aktualizuje dostupná rozlišení grafu podle velikosti naměřených hodnot
  protected static update_char_available_resolutions(): void {
    //Uložení dostupných rozlišení pro graf
    let available_resolutions: number[] = [...CHAR_VIEW_RESOLUTION_OPTIONS];
    //Uložení celého grafu do konstantní proměnné
    const data = this.fridge_data.json_graph_chars_format;
    //Pokud existují data, provede se následující
    if(data) {
      //TODO DELETE
      // //Filtrování pro dostupná rozlišení
      // available_resolutions = available_resolutions.filter((value: number, index: number) => {
      //   return value < data[0].series.length || !index;
      // });

      //Upravíme a vratíme objekt ve travu, který potřebujeme `${hodnota v minutách} minuta`
      this.fridge_data.json_graph_resolution_view = available_resolutions.map((value: number, index: number) => {
        return {value: `${value} minut`, enabled: (value < data[0].series.length + 1 || !index)};
      });
    }
  }

  //Statická funkce, která vrátí dostupná rozlišení grafu
  public static get_char_available_resolutions(): NapicuOptionsData[] {
    return this.fridge_data.json_graph_resolution_view?.length ? this.fridge_data.json_graph_resolution_view : CHAR_VIEW_RESOLUTION_OPTIONS.map((value: number, index: number) => {
      return {value: `${value} minut`, enabled: false};
    });
  }

  //Statická funkce, která zkopíruje všechna naměřená data do schránky zařízení (Pokud se vrátí null, data neexistují )
  public static copy_json_data_to_clipboard(): Promise<void> | null {
    //Získání naměřených dat
    const data: CharTempsData | null = this.get_full_json_temp_char();

    //Pokud data existují provede se následující
    if(data) {
      //Úprava struktury dat pro kopírování
      const updatedData = data.map(item => {
        const { series, ...rest } = item;
        const updatedSeries = series.map(({ name, value }) => ({ value }));
        return { ...rest, series: updatedSeries };
      });

      return Clipboard.write({
        string: JSON.stringify(updatedData)
      });
    }
    return null;
  }

  //Statická funkce, která nastaví kolit dat naměřených teplot, se mají zobrazit v grafu
  public static set_char_resolution(index: number): void {
    this.fridge_data.char_settings.display_resolution = index;
    //Spuštění funkce pro uložení nastavení grafu
    this.save_char_settings();
    //Spuštění funkce pro aktualizování rozsahu dat
    this.update_char_view_data()
  }

  //Statická funkce, která vrátí barvu LED osvětlení
  public static get_led_color(): RGBA {
    return this.fridge_data.config.fridge_led_rgb;
  }

  //Statická funkce, která vrátí zda při chybě bude bzučet piezo
  public static get_buzzing_on_error(): boolean {
    return this.fridge_data.config.buzzing_on_error;
  }

  //Statická funkce, která vrátí zda došlo v ledničce k problému (zařízení musí být připojené, jinak se vrátí log0)
  public static get_is_fridge_on_error(): boolean {
    return (this.fridge_data.errors.fridge_in_temp
      || this.fridge_data.errors.fridge_out_temp
      || this.fridge_data.errors.fridge_cooler_temp
      ||this.fridge_data.errors.fridge_fan
      ) && this.is_connected();
  }

  //Statická funkce, která vrátí zda je vnitřní teploměr v chybě
  public static get_is_in_temp_in_error(): boolean {
    return this.fridge_data.errors.fridge_in_temp;
  }

  //Statická funkce, která vrátí zda je venkovní teploměr v chybě
  public static get_is_out_temp_in_error(): boolean {
    return this.fridge_data.errors.fridge_out_temp;
  }

  //Statická funkce, která vrátí zda je teploměr na chladiči v chybě
  public static get_is_cooler_temp_in_error(): boolean {
    return this.fridge_data.errors.fridge_cooler_temp;
  }

  //Statická funkce, která vrátí zda jsou ventilátory v chybě
  public static get_is_fans_in_error(): boolean {
    return this.fridge_data.errors.fridge_fan;
  }

  //Statická funkce, která vrátí zda došlo v ledničce k vážné poruše
  public static get_is_fridge_on_fatal_error(): boolean {
    return this.fridge_fatal_error;
  }

  //Statická funkce, která vrátí data naměřených teplot, které se mají zobrazit v grafu
  public static get_char_view_data(): CharTempsData | null {
    return this.fridge_data.json_graph_chars_format_view;
  }

  //Statická funkce, která vrátí kolik hodnot se má zobrazit v grafu
  public static get_char_resolution(): number {
    return CHAR_VIEW_RESOLUTION_OPTIONS[this.fridge_data.char_settings.display_resolution];
  }

  //Statická funkce, která vrátí index jaká hodnota je vybraná v CHAR_VIEW_RESOLUTION_OPTIONS
  public static get_char_resolution_index(): number {
    return this.fridge_data.char_settings.display_resolution;
  }

  //Statická funkce, která vrátí zda se má lednička pozastavit při otevřených dveří
  public static get_fridge_stop_on_open_door(): boolean {
    return this.fridge_data.config.fridge_stop_on_open_door;
  }

  //Statická funkce, která vrátí zda se má LED osvětlení zapnout při otevřených dveří
  public static get_fridge_led_enable(): boolean {
    return this.fridge_data.config.fridge_led_enable;
  }

  //Statická funkce, která vrátí hodnotu jasu LED osvětlení (0-100)
  public static get_fridge_led_brightness(): number {
    return this.fridge_data.config.fridge_led_brightness;
  }

  //Statická funkce, která přidá barvu do oblíbených barev osvětlení
  public static add_user_favorite_color(color: RGBA): void {
    //Přidání nové barvy do pole uložených barev
    this.fridge_data.user_favorites_colors.push(color)
    //Uložení nastavení
    AppComponent.application_settings.setItem("favourites_colors_led", JSON.stringify(this.fridge_data.user_favorites_colors));
  }

  //Statická funkce, která vrátí uložené oblíbené barvy
  protected static get_user_favorites_colors_from_storage(): RGB[] {
    //Získání uložených dat
    let i: string | null = AppComponent.application_settings.getItem("favourites_colors_led");
    //Pokud existuje uložená hodnota provede se následující
    if(i) return JSON.parse(i) as RGB[];
    //Vrácení výchozích hodnot pokud uložená data neexistují
    return this.fridge_data.user_favorites_colors;
  }

  //Statická funkce, která odebere oblíbenou barvu osvětlení podle indexu
  public static remove_user_favorite_color(index: number): void {
    //Vymazání hodnoty z objektu
    this.fridge_data.user_favorites_colors.splice(index, 1);
    //Uložení nastavení
    AppComponent.application_settings.setItem("favourites_colors_led", JSON.stringify(this.fridge_data.user_favorites_colors));
  }

  //Statická funkce, která vrátí zda se má nápověda k odstranění oblíbené barvy zobrazit z uloženého nastavení
  protected static get_is_delete_color_hint_enabled_from_storage(): boolean {
    //Získání uložených dat
    let i: string | null = AppComponent.application_settings.getItem("favourites_colors_led_hint");
    //Pokud existuje uložená hodnota provede se následující
    if(i) return JSON.parse(i) as boolean;
    //Vrácení výchozích hodnot pokud uložená data neexistují
    return this.fridge_data.user_delete_color_hint;
  }

  //Statická funkce, která vypne zobrazování nápovědy k odstranění oblíbené barvy
  public static disable_favorites_colors_hint(): void {
    //Nastavení proměnné pro zobrazování nápovědy na log0
    this.fridge_data.user_delete_color_hint = false;
    //Uložení nastavení
    AppComponent.application_settings.setItem("favourites_colors_led_hint", "false");
  }

  //Statická funkce, která vrátí zda se má nápověda k odstranění oblíbené barvy zobrazit
  public static get_is_delete_color_hint_enabled(): boolean {
    return this.fridge_data.user_delete_color_hint;
  }

  //Statická funkce, která vrátí zda se má nápověda ke grafu zobrazit z uloženého nastavení
  protected static get_is_charts_hint_enabled_from_storage(): boolean {
    //Získání uložených dat
    let i: string | null = AppComponent.application_settings.getItem("charts_hint");
    //Pokud existuje uložená hodnota provede se následující
    if(i) return JSON.parse(i) as boolean;
    //Vrácení výchozích hodnot pokud uložená data neexistují
    return this.fridge_data.user_char_hint;
  }

  //Statická funkce, která vypne zobrazování nápovědy ke grafu
  public static disable_charts_hint(): void {
    //Nastavení proměnné pro zobrazování nápovědy na log0
    this.fridge_data.user_char_hint = false;
    //Uložení nastavení
    AppComponent.application_settings.setItem("charts_hint", "false");
  }

  //Statická funkce, která vrátí zda se má nápověda ke grafu zobrazit
  public static get_is_charts_hint_enabled(): boolean {
    return this.fridge_data.user_char_hint;
  }

  //Statická funkce, která vrátí oblíbené barvy
  public static get_user_favorites_colors(): RGB[] {
    return this.fridge_data.user_favorites_colors;
  }

  //Statická funkce, která vymaže data zobrazující se v grafu
  public static clear_char_view_data(): void {
    this.fridge_data.json_graph_chars_format_view = null;
    this.fridge_data.json_graph_resolution_view = null;
  }

  //Statická funkce, která nastaví obrácenou bool hodnotu proměnné určující zobrazení křivky vnitřní teploty na grafu
  public static switch_in_temp_display_char(): void {
    this.fridge_data.char_settings.display_in_temp = !this.fridge_data.char_settings.display_in_temp;
    //Spuštění funkce pro uložení nastavení grafu
    this.save_char_settings();
    //Spuštění funkce pro aktualizovaní dat, které se mají zobrazit v grafu
    this.update_char_view_data();
  }

  //Statická funkce, která nastaví obrácenou bool hodnotu proměnné určující zobrazení křivky venkovní teploty na grafu
  public static switch_out_temp_display_char(): void {
    this.fridge_data.char_settings.display_out_temp = !this.fridge_data.char_settings.display_out_temp;
    //Spuštění funkce pro uložení nastavení grafu
    this.save_char_settings();
    //Spuštění funkce pro aktualizovaní dat, které se mají zobrazit v grafu
    this.update_char_view_data();
  }

  //Statická funkce, která nastaví obrácenou bool hodnotu proměnné určující zobrazení křivky teploty na chladiči v grafu
  public static switch_cooler_temp_display_char(): void {
    this.fridge_data.char_settings.display_cooler_temp = !this.fridge_data.char_settings.display_cooler_temp;
    //Spuštění funkce pro uložení nastavení grafu
    this.save_char_settings();
    //Spuštění funkce pro aktualizovaní dat, které se mají zobrazit v grafu
    this.update_char_view_data();
  }

  //Statická funkce která vrátí čas ve kterém se naposledy aktualizoval graf naměřených hodnot
  public static get_char_last_update_time(): number {
    return this.json_graph_last_update_date;
  }

  //Statická funkce která vrátí čas ve kterém se naposledy aktualizoval graf naměřených hodnot v základním formátu
  public static get_char_last_update_basic_format(): string { //TODO Optimize
    return new NapicuDate(this.get_char_last_update_time()).format(CHAR_LAST_UPDATE_DATE_FORMAT);
  }
}
