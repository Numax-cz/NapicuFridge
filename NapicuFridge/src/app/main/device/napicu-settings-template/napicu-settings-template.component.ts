import {Component, Input, OnInit} from '@angular/core';
import {Router} from "@angular/router";

@Component({
  selector: 'app-napicu-settings-template',
  templateUrl: './napicu-settings-template.component.html',
  styleUrls: ["../../main.page.scss"],
})
export class NapicuSettingsTemplateComponent {


  @Input() redirect: string = "/main/device";

  constructor(private router: Router) { }

  //Funkce, která vrátí uživatele na předchozí stránku
  public back(): void {
    this.router.navigate([this.redirect]);
  }
}
