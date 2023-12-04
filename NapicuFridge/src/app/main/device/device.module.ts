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
import {NapicuOptionTemplateComponent} from "./napicu-option-template/napicu-option-template.component";
import {LightingComponent} from "./lighting/lighting.component";
import {NgxSliderModule} from "ngx-slider-v2";
import {ColorSketchModule} from "ngx-color/sketch";

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    DevicePageRoutingModule,
    BrowserAnimationsModule,
    LineChartModule,
    NgSelectModule,
    FormsModule,
    NgxSliderModule,
    ColorSketchModule
  ],
  declarations: [DevicePage, InformationComponent, SettingsComponent, DisplayComponent, ChartsComponent, LightingComponent ,NapicuSettingsTemplateComponent, NapicuOptionTemplateComponent]
})
export class DevicePageModule {}
