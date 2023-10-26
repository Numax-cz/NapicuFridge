import {Component, Input, NgZone} from '@angular/core';
import {show_box_animations} from "../../Animation";
import {NapicuOptionsData} from "../../../interface/NapicuOption";

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
  @Input() options: NapicuOptionsData[] = [];
  //Proměnná, která určuje která možnost je vybraná
  @Input() selected: number = 0;
  //Proměná pro uložení zpětného volání
  @Input() on_change: ((index: number) => void) | null = null;
  //Proměnná, která určuje zda je povoleno ovládání
  @Input() disabled: boolean = false;

  constructor(private ngZone: NgZone) { }

  public set_active_menu(value: boolean): void {
    if(this.disabled) return;
    //Spuštění funkce uvnitř zóny Angularu
    this.ngZone.run(() => {
      this.active_menu = value;
    });
  }

  //Funkce, která se spustí po kliknutí na možnost
  public on_click_option(value: NapicuOptionsData, index: number, event: any): void {
    //Pokud je hodnota k dispozici pro vybrání
    if(value.enabled) {
      this.selected = index;
      //Spuštění funkce pro zpětné volání
      this.on_change?.(index);
    } else {
      //Spuštění funkce která zabrání tomu, aby byla volána stejná událost (on_click event)
      event.stopPropagation();
    }
  }

  //Funkce, která vrátí vybranou možnost
  public get_selected_option(): string {
    return this.options[this.selected].value;
  }
}
