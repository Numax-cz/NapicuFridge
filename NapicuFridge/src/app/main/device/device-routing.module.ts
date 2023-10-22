import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import {InformationComponent} from "./information/information.component";
import {SettingsComponent} from "./settings/settings.component";
import {DisplayComponent} from "./display/display.component";
import {ChartsComponent} from "./charts/charts.component";



const routes: Routes = [
  {
    path: "device-information",
    component: InformationComponent,
  },
  {
    path: "device-settings",
    component: SettingsComponent,
  },
  {
    path: "device-display",
    component: DisplayComponent,
  },
  {
    path: "device-data",
    component: ChartsComponent,
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class DevicePageRoutingModule {}
