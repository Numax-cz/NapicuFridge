import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { DevicePageRoutingModule } from './device-routing.module';

import { DevicePage } from './device.page';
import {InformationComponent} from "./information/information.component";
import {NapicuSettingsTemplateComponent} from "./napicu-settings-template/napicu-settings-template.component";
import {SettingsComponent} from "./settings/settings.component";
import {BrowserAnimationsModule} from "@angular/platform-browser/animations";
import {DisplayComponent} from "./display/display.component";

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    DevicePageRoutingModule,
    BrowserAnimationsModule
  ],
  declarations: [DevicePage, InformationComponent, SettingsComponent, DisplayComponent ,NapicuSettingsTemplateComponent]
})
export class DevicePageModule {}
