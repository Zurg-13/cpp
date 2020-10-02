import { Component, OnInit } from '@angular/core';
import { ItemService } from 'src/app/srv/item.service';
import { WsctService } from 'src/app/srv/wsct.service';
import { Subscription } from 'rxjs';

/* Писок Item'ов. *************************************************************/
/******************************************************************************/
@Component({
  selector: 'app-ctrl',
  templateUrl: './ctrl.component.html',
  styleUrls: ['./ctrl.component.scss']
})
export class CtrlComponent implements OnInit {

  private subs: Subscription;

  constructor(
    public item_svc: ItemService
  , public wsct_svc: WsctService  
  ) {}

  // Выполняется пр активации. -------------------------------------------------
  //----------------------------------------------------------------------------
  ngOnInit(): void { 
    this.subs ? this.subs :
    this.wsct_svc.openObserver.subscribe(() => { 
      this.item_svc.load_list(); 
      this.subs.unsubscribe();
    });
  }// ngOnInit

}// CtrlComponent
