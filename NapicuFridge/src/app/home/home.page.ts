import {Component} from '@angular/core';

import {BLE} from "@awesome-cordova-plugins/ble";
import {AppVersion} from "@awesome-cordova-plugins/app-version";
import {AppComponent} from "../app.component";

interface data {
  name: string,
  id: string,
  advertising: ArrayBuffer,
  rssi: number,
}


@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage {

  public devices: data[] = [
    {
      name: "NapicuFridge",
      id: "fjf",
      rssi: 32,
      advertising: new ArrayBuffer(12)
    },
    {
      name: "NapicuFridge2",
      id: "fjf",
      rssi: 32,
      advertising: new ArrayBuffer(12)
    }
  ];

  public scanning: boolean = false;


  constructor() {
  }

  public auto_connect(): void {

  }

  public scan(): void {

    this.scanning = true;


    BLE.startScan(["6E400001-B5A3-F393-E0A9-E50E24DCCA9E"]).subscribe(
      {
        next: (data: data) => {
          this.devices.push(data);
        },
        error: (data) => {
          console.log(data);
        }

      }
    )


    //TODO END scan + services

  }

  public stop_scan(): void {
    BLE.stopScan();
    this.scanning = false;
  }

  public on_click_device(device_id: number): void {
    this.stop_scan();
  }

  public get_app_version(): string {
    return `${AppComponent.application_name} ${AppComponent.application_version_code}`;
  }

}
