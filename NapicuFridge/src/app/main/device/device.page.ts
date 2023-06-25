import { Component } from '@angular/core';
import {BluetoothLE} from "@awesome-cordova-plugins/bluetooth-le";


@Component({
  selector: 'app-device',
  templateUrl: './device.page.html',
  styleUrls: ['./device.page.scss', '../main.page.scss'],
})
export class DevicePage {

  constructor() {
  }






  public get_mac_address(): string {
    return '4C-54-AE-FF-66-E0';
  }

}
