import {Component} from '@angular/core';

import {BLE} from "@awesome-cordova-plugins/ble";

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

  public devicess: data[] = [];

  public error: any = "";

  constructor() {
  }

  public scan(): void {




    BLE.startScan(["6E400001-B5A3-F393-E0A9-E50E24DCCA9E"]).subscribe(
      {
        next: (data: data) => {
          this.devicess.push(data);
        },
        error: (data) => {
          console.log(data);
        }

      }
    )


    //TODO END scan + services

  }

}
