import {Component} from '@angular/core';
import {Platform} from "@ionic/angular";
import {StatusBar, Style} from "@capacitor/status-bar";

@Component({
  selector: 'app-root',
  templateUrl: 'app.component.html',
  styleUrls: ['app.component.scss'],
})
export class AppComponent {
  constructor(private platform: Platform) {
    platform.ready().then(() => {
      //Kontrola zda je zařízení typu android
      if (platform.is('android')) {
        //Nastavení barvy textu horního status baru v mobilní aplikaci
        StatusBar.setStyle({style: Style.Light});
        //Nastavení barvy horního statu sbaru v mobilní aplikaci
        StatusBar.setBackgroundColor({color: "#ffffff"});
      }
    });
  }
}
