import { Component, OnInit } from '@angular/core';
import {AppComponent} from "../../../app.component";

@Component({
  selector: 'app-information',
  templateUrl: './information.component.html',
  styleUrls: ['./information.component.scss', "../../main.page.scss"],
})
export class InformationComponent {

  constructor() { }


  //Funkce, která vrátí jméno připojeného zařízení
  public get_device_name(): string {
    return AppComponent.get_paired_device_name();
  }

  //Funkce, která vrátí adresu MAC připojeného zařízení
  public get_device_address(): string {
    return AppComponent.get_paired_device_address();
  }

}
