import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import {InformationComponent} from "./information/information.component";
import {SettingsComponent} from "./settings/settings.component";



const routes: Routes = [
  {
    path: "device-information",
    component: InformationComponent,
  },
  {
    path: "device-settings",
    component: SettingsComponent,
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class DevicePageRoutingModule {}
