import { Injectable } from '@angular/core';
import { WsctService } from './wsct.service';
import { Cmnd } from '../cls/cmnd';
import { Room } from '../cls/room';
import { Item } from '../cls/Item';
import { ThrowStmt } from '@angular/compiler';

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
          this.setItem(data.list);
        } break;

      }// switch(data.cmnd)
    });
  }// constructor

  // Вернуть размещение (создав новое, если отсутствует). ----------------------
  //----------------------------------------------------------------------------
  private putRoom(item: Item): void {

    // Размещение существует.
    for(let i=0; i<this.menu.length; i++) { 
      if(item.room === this.menu[i].id) 
        { this.menu[i].itm.push(item); return; } 
    }// i
    
    // Размещение НЕ существует.
    let room = new Room(item.room, item.rnme, "#отсутствует#");
    room.itm.push(item);
    this.menu.push(room);

  }// getRoom

  // Разместить item'ы согласно размещению. ------------------------------------
  //----------------------------------------------------------------------------
  private setItem(list: Item[]) 
    { list.forEach((item: Item) => { this.putRoom(item); }); }

  // Запросить список элементов. -----------------------------------------------
  //----------------------------------------------------------------------------
  public load_list(): void { this.wsct_svc.send(new Cmnd("item_list", null)); }

}// MenuService
