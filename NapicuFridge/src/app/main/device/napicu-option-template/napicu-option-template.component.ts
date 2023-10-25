import {Component, Input} from '@angular/core';
import {show_box_animations} from "../../Animation";

@Component({
  selector: 'app-napicu-option-template',
  templateUrl: './napicu-option-template.component.html',
  styleUrls: ['./napicu-option-template.component.scss'],
  animations: [show_box_animations]
})
export class NapicuOptionTemplateComponent {

  //Proměnná, která určuje zda je menu pro vybrání možnosti otevřen
  public active_menu: boolean = false;
  //Proměnná, která uloží možnosti
  @Input() options: string[] = [];
  //Proměnná, která určuje která možnost je vybraná
  @Input() selected: number = 0;
  //Proměná pro uložení zpětného volání
  @Input() on_change: ((index: number) => void) | null = null;

  constructor() { }

  //Funkce, která vrátí vybranou možnost
  public get_selected_option(): string {
    return this.options[this.selected];
  }
}
