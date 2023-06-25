import {Component} from '@angular/core';
import {Platform} from "@ionic/angular";
import {StatusBar, Style} from "@capacitor/status-bar";
import {AppVersion} from "@awesome-cordova-plugins/app-version";
import {App} from "@capacitor/app";
import {BluetoothLE, CurrConnectionStatus} from "@awesome-cordova-plugins/bluetooth-le";

@Component({
  selector: 'app-root',
  templateUrl: 'app.component.html',
  styleUrls: ['app.component.scss'],
})
export class AppComponent {

  //Statická proměnná pro local storage nastavení aplikace
  public static application_settings: Storage = window.localStorage;

  //Statická proměnná pro uložení jména aplikace
  public static application_name: string | null = null;

  //Statická proměnná pro uložení verze aplikace
  public static application_version_code: string | null = null;

  //Statická proměnná pro uložení ID připojeného zařízení
  public static connected_device: string | null = null;

  constructor(private platform: Platform) {
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
        //Nastavení barvy horního statu sbaru v mobilní aplikaci
        StatusBar.setBackgroundColor({color: "#ffffff"});

        //Získání jména aplikace
        AppVersion.getAppName().then((value: string) => AppComponent.application_name = value);
        //Získání verze aplikace
        AppVersion.getVersionNumber().then((value: string) => AppComponent.application_version_code = value);

        //Zamknutí orientace aplikace (na výšku)
        screen.orientation.lock("portrait");
      }
    });
  }

  //Funkce, která vrátí zda je zařízení připojené
  public static async is_connected(): Promise<boolean> {
    if (this.connected_device) return await BluetoothLE.isConnected({address: this.connected_device}).then((status: CurrConnectionStatus) => status.isConnected);
    return false;
  }
}
