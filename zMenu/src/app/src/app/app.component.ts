import { Component, OnInit } from '@angular/core';
import { ConfigService } from './srv/config.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {

  constructor(
    private configService: ConfigService
  ) {

  }

  ngOnInit() {
   
  }
}
