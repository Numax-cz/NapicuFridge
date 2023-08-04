import {Component, NgZone, OnInit} from '@angular/core';
import {AppComponent} from "../../app.component";
import {alert_animations} from "../Animation";



@Component({
  selector: 'app-info',
  templateUrl: './info.page.html',
  styleUrls: ['./info.page.scss', '../main.page.scss'],
  animations: alert_animations
})
export class InfoPage {

  public is_info_icon_alert_activated: boolean = false;


  constructor() {
  }

  public get_in_temp(): string {
    return AppComponent.get_in_temp();
  }
}
