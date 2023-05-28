import { Component, OnInit  } from '@angular/core';
import {BluetoothSerialOriginal} from "@awesome-cordova-plugins/bluetooth-serial";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit   {

  public lists: string[] = [];

  constructor(public btSerial: BluetoothSerialOriginal){
  }

  ngOnInit(): void {


  }



}
