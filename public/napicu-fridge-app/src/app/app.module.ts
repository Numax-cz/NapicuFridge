import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import {BluetoothSerialOriginal} from "@awesome-cordova-plugins/bluetooth-serial";

@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule
  ],
  providers: [
    BluetoothSerialOriginal
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
