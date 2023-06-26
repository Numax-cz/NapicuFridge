import { Component } from '@angular/core';
import {BluetoothLE} from "@awesome-cordova-plugins/bluetooth-le";
import {AppComponent} from "../../app.component";


@Component({
  selector: 'app-device',
  templateUrl: './device.page.html',
  styleUrls: ['./device.page.scss', '../main.page.scss'],
})
export class DevicePage {

  constructor() {
  }




  public is_connected(): boolean {
    return AppComponent.is_connected();
  }


  public get_mac_address(): string {
    return AppComponent.connected_device?.address || "";
  }

}
