import { Component, OnInit } from '@angular/core';
import { ConfService } from './srv/conf.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {

  constructor(
    private conf_svc: ConfService
  ) {

  }

  ngOnInit() {
   
  }
}
