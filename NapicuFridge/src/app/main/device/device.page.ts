import { Component } from '@angular/core';
import {BluetoothLE} from "@awesome-cordova-plugins/bluetooth-le";
import {AppComponent} from "../../app.component";
import {animate, state, style, transition, trigger} from "@angular/animations";


@Component({
  selector: 'app-device',
  templateUrl: './device.page.html',
  styleUrls: ['./device.page.scss', '../main.page.scss'],
  animations: [
    trigger('AlertAnimation', [
      state('void', style({ opacity: 0, transform: 'translate(-50%, 200%)' })),
      transition(':enter', [
        animate(200, style({ opacity: 1, transform: 'translate(-50%, 0%)' }))
      ]),
      transition(':leave', [
        animate(200, style({ opacity: 1, transform: 'translate(-50%, 200%)' }))
      ]),

    ]),

    trigger('AlertAnimationBackground', [
      state('void', style({ opacity: 1 })),

      transition(':leave', [
        animate(400, style({ opacity: 0 }))
      ]),

    ]),
  ],
})
export class DevicePage {

  public active_alert: boolean = true;


  constructor() {
  }


  public on_click_factory_reset(): void {
    this.active_alert = true;
  }

  //Tato funkce obnoví tovární nastavení. Funkce se spustí po kliknutí na tlačítko
  public factory_reset(): void {
    AppComponent.factory_reset();
  }


  //Funkce, která vrátí zda je zařízení připojené
  public is_connected(): boolean {
    return AppComponent.is_connected();
  }


  //Funkce, která vrátí adresu MAC připojeného zařízení
  public get_mac_address(): string {
    return AppComponent.connected_device?.address || "";
  }
}
