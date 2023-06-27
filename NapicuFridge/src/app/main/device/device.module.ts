import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { DevicePageRoutingModule } from './device-routing.module';

import { DevicePage } from './device.page';
import {InformationComponent} from "./information/information.component";
import {NapicuSettingsTemplateComponent} from "./napicu-settings-template/napicu-settings-template.component";
import {SettingsComponent} from "./settings/settings.component";

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    DevicePageRoutingModule
  ],
  declarations: [DevicePage, InformationComponent, SettingsComponent,NapicuSettingsTemplateComponent]
})
export class DevicePageModule {}
