import {Component, OnInit} from '@angular/core';
import {BluetoothSerial} from "@ionic-native/bluetooth-serial";
import {AndroidPermissions} from "@awesome-cordova-plugins/android-permissions";
import {AlertController, NavController, ToastController} from "@ionic/angular";

interface pairedlist {
  "class": number,
  "id": string,
  "address": string,
  "name": string
}


@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage {

  public unpairedDevices: any;
  public pairedDevices: any;
  public declare gettingDevices: Boolean;

  constructor(
    public alertCtrl: AlertController
  ) {
    let obj = this;

    // Check, if Bluetooth is enabled:
    BluetoothSerial.isEnabled().then(
      (result: any) => {
        console.log(result);

      },
      (error: any) => {
        console.log(error);

        obj._enableBluetooth();
      }
    );
  }

  private _enableBluetooth(): void {
    BluetoothSerial.enable().then(
      (success: any) => {
        console.log(success);

      },
      (error: any) => {
        console.log(error);
      }
    );
  }

  public startScanning(): void {
    this.pairedDevices = null;
    this.unpairedDevices = null;
    this.gettingDevices = true;

    BluetoothSerial.discoverUnpaired().then(
      (result: any) => {
        console.log(result);

        this.unpairedDevices = result;
        this.gettingDevices = false;

        result.forEach((element: any) => {
          console.log(element);
        });
      },
      (error: any) => {
        console.log(error);
      }
    );

    BluetoothSerial.list().then(
      (result: any) => {
        console.log(result);

        this.pairedDevices = result;
      },
      (error: any) => {
        console.log(error);
      }
    );
  }

  public selectDevice(address: any): any {
    // console.log(address);
    //
    // let alert = this.alertCtrl.create({
    //   title:    'Connect?',
    //   message:  'Do you want to connect to the device?',
    //   buttons:  [
    //     {
    //       text: 'Cancel',
    //       role: 'cancel',
    //       handler: () => {
    //         console.log('Cancel clicked.');
    //       }
    //     },
    //     {
    //       text: 'Connect',
    //       handler: () => {
    //         BluetoothSerial.connectInsecure(address).subscribe(this._success, this._fail);
    //       }
    //     }
    //   ]
    // });
    // alert.present();
  }

  private _success = (data: any) => console.log(data);

  private _fail = (error: any) => console.log(error);

  public async disconnect() {
    // let alert = this.alertCtrl.create({
    //   title:    'Disconnect?',
    //   message:  'Do you want to disconnect from the device?',
    //   buttons:  [
    //     {
    //       text: 'Cancel',
    //       role: 'cancel',
    //       handler: () => {
    //         console.log('Cancel clicked.');
    //       }
    //     },
    //     {
    //       text: 'Disconnect',
    //       handler: () => {
    //         BluetoothSerial.disconnect();
    //       }
    //     }
    //   ]
    // });
    // alert.present();
  }

}
