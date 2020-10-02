import { Injectable } from '@angular/core';
import { WsctService } from './wsct.service';
import { Cmnd } from '../cls/cmnd';
import { Room } from '../cls/room';
import { Item } from '../cls/Item';

@Injectable({ providedIn: 'root' })

/* Управление меню. ***********************************************************/
/******************************************************************************/
export class MenuService {

  public menu: Room[] = [];


  // Конструктор. --------------------------------------------------------------
  //----------------------------------------------------------------------------
  constructor(
    private wsct_svc: WsctService
  ) {
    this.wsct_svc.onMessageObserver.subscribe((data: any) => {
      switch(data.cmnd) {

        case "item_list": {

          for(let i=1; i<5; i++) {
            let itm: Item[] = [];
            let room = new Room(i, "name_"+i, "note_"+i)
            for(let j=1; j<5; j++) {
              itm.push(new Item(i*j, "dish_"+j, "note_"+j, j*10
              , null, null, null, null, null, null));
            }
            room.itm = itm;
            this.menu.push(room);
          }

        } break;

      }// switch(data.cmnd)
    });

  }

  public load_list(): void { this.wsct_svc.send(new Cmnd("item_list", null)); }

}// MenuService
