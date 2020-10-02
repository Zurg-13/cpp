import { Component, OnInit, Input } from '@angular/core';
import { Room } from 'src/app/cls/room';
import { Subscription } from 'rxjs';
import { WsctService } from 'src/app/srv/wsct.service';
import { MenuService } from 'src/app/srv/menu.service';

@Component({
  selector: 'app-menu',
  templateUrl: './menu.component.html',
  styleUrls: ['./menu.component.scss']
})
export class MenuComponent implements OnInit {

  subs: Subscription

  constructor(
    public wsct_svc: WsctService
  , public menu_svc: MenuService
  ) { }

  ngOnInit(): void {
    this.subs ? this.subs :
    this.wsct_svc.openObserver.subscribe(() => { 
      this.menu_svc.load_list(); 
      this.subs.unsubscribe();
    });

  }

}

