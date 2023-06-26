import {Component, NgZone, OnInit} from '@angular/core';
import {AppComponent} from "../../app.component";



@Component({
  selector: 'app-info',
  templateUrl: './info.page.html',
  styleUrls: ['./info.page.scss', '../main.page.scss'],
})
export class InfoPage {



  constructor() {

  }

  public get_in_temp(): string {
    return AppComponent.get_in_temp();
  }
}
