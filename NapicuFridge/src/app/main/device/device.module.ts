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
import {ChartsComponent} from "./charts/charts.component";
import {LineChartModule} from "@swimlane/ngx-charts";
import {NgSelectModule} from "@ng-select/ng-select";

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    DevicePageRoutingModule,
    BrowserAnimationsModule,
    LineChartModule,
    NgSelectModule,
    FormsModule
  ],
  declarations: [DevicePage, InformationComponent, SettingsComponent, DisplayComponent, ChartsComponent ,NapicuSettingsTemplateComponent]
})
export class DevicePageModule {}
