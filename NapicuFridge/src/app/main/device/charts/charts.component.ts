import { Component, OnInit } from '@angular/core';
import {AppComponent} from "../../../app.component";

@Component({
  selector: 'app-charts',
  templateUrl: './charts.component.html',
  styleUrls: ['./charts.component.scss'],
})
export class ChartsComponent  {
  public preview_graph_data: {name: string, series: {value: number, name: string}[]}[] = AppComponent.fridge_data.json_graph_chars_format;

  constructor() { }
}
