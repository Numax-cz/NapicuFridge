import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { MainPage } from './main.page';
import {InfoPage} from "./info/info.page";
import {InfoPageModule} from "./info/info.module";
import {DevicePage} from "./device/device.page";
import {DevicePageModule} from "./device/device.module";
import {InformationComponent} from "./device/information/information.component";

const routes: Routes = [
  {
    path: 'main',
    component: MainPage,
    children: [
    {
      path: 'info',
      //loadChildren: () => import('./info/info.module').then( m => m.InfoPageModule)
      component: InfoPage
    },
    {
      path: 'device',
      component: DevicePage,
    },
    ],
  },

];

@NgModule({
  imports: [RouterModule.forRoot(routes), InfoPageModule, DevicePageModule],
  exports: [RouterModule],
})
export class MainPageRoutingModule {}
