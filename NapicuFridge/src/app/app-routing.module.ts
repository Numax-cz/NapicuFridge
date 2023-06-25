import { NgModule } from '@angular/core';
import { PreloadAllModules, RouterModule, Routes } from '@angular/router';
import {MainPage} from "./main/main.page";
import {MainPageModule} from "./main/main.module";

const routes: Routes = [
  {
    path: 'home',
    loadChildren: () => import('./home/home.module').then( m => m.HomePageModule)
  },
  // {
  //   path: 'main',
  //   // loadChildren: () => import('./main/main.module').then( m => m.MainPageModule)
  //   component: MainPage
  // },
  {
    path: '',
    redirectTo: 'home',
    pathMatch: 'full'
  },
];

@NgModule({
  imports: [
    RouterModule.forRoot(routes, { preloadingStrategy: PreloadAllModules }),
    MainPageModule
  ],
  exports: [RouterModule]
})
export class AppRoutingModule { }
