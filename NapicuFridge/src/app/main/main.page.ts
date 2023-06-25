import {Component, NgZone} from '@angular/core';
import {AppComponent} from "../app.component";
import {Configuration} from "../config/configuration";
import {
  AndroidGattTransportMode,
  BluetoothLE,
  Device,
  OperationResult
} from "@awesome-cordova-plugins/bluetooth-le";
import CHARACTERISTIC_UUID_TX = Configuration.CHARACTERISTIC_UUID_TX;
import {Router} from "@angular/router";



@Component({
  selector: 'app-main',
  templateUrl: './main.page.html',
  styleUrls: ['./main.page.scss'],
})
export class MainPage  {

  constructor(public router: Router) {

  }


  public redirect(url: string): void {
    this.router.navigateByUrl(`/${url}`);
  }
}
