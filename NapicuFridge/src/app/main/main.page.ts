import {Component} from '@angular/core';
import {Router} from "@angular/router";
import {AppComponent} from "../app.component";

@Component({
  selector: 'app-main',
  templateUrl: './main.page.html',
  styleUrls: ['./main.page.scss'],
})
export class MainPage  {

  constructor(public router: Router) { }

  public redirect(url: string): void {
    this.router.navigateByUrl(`/${url}`);
  }

  //Funkce, která vrátí zda došlo v ledničce k problému
  public get_is_fridge_on_error(): boolean {
    return AppComponent.get_is_fridge_on_error();
  }
}
