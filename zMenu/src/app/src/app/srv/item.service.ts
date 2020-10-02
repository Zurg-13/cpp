import { Injectable } from '@angular/core';
import { Item } from '../cls/Item';
import { WsctService } from './wsct.service';
import { Cmnd } from '../cls/cmnd';

@Injectable({ providedIn: 'root' })

/* Модель ленты элементов меню. ***********************************************/
/******************************************************************************/
export class ItemService {

  public list: Item[] = []; // Список редактируемых карточек item
  public slct: Item;

  // Конструктор. --------------------------------------------------------------
  //----------------------------------------------------------------------------
  constructor(
    private wsct_svc: WsctService
  ) {
    this.wsct_svc.onMessageObserver.subscribe((data: any) => {
      switch(data.cmnd) {

        case "item_list": {
          this.setList(data.list)
        } break;

        case "item_post": {
          let item = this.getItem(data.id);
          if(item) { item.id = data.sign; }
        } break;

        case "item_save": {
          let item = this.getItem(data.id);
          if(item) { console.log("!!! СОХРАНИТЬ СОСТОЯНИЕ !!!"); }
        } break;

        case "item_drop": {
          let item = this.getItem(data.id);
          if(item) { this.delItem(item.id); }
        } break;

      }// switch(data.cmnd)
    });
  }// constructor

  // Загрузить список элементов. -----------------------------------------------
  //----------------------------------------------------------------------------
  public load_list(): void { 
    console.log("item_list");
    this.wsct_svc.send(new Cmnd("item_list", null)); 
  }

  // Управление списком элементов. ---------------------------------------------
  //----------------------------------------------------------------------------
  public getList(): Item[] { return this.list; }
  public setList(list: Item[]) { this.list = list; }

  // Вернуть элемент. ----------------------------------------------------------
  //----------------------------------------------------------------------------
  public getItem(id: number): Item { 
    for(let item of this.list) { if(item.id == id) { return item; }}
    return null;
  }// getItem

  // Вставить элемент. ---------------------------------------------------------
  //----------------------------------------------------------------------------
  public insItem(id: number, item: Item) {
    for(let i=0; i<this.list.length; i++) 
      { if(this.list[i].id == id) { this.list.splice(i, 0, item); return; }}
  }// insItem

  // Удалить элемент. ----------------------------------------------------------
  //----------------------------------------------------------------------------
  public delItem(id: number) {
    for(let i=0; i<this.list.length; i++) 
      { if(this.list[i].id == id) { this.list.splice(i, 1); return; }}    
  }// delItem

  // Обновить состояние атрибутов. ---------------------------------------------
  //----------------------------------------------------------------------------
  public updItemRoom(id: number, r_id: number, name: string) 
    { let item = this.getItem(id); item.room = r_id; item.rnme = name;}
  public updItemType(id: number, t_id: number, name: string) 
    { let item = this.getItem(id); item.type = t_id; item.tnme = name; }
    
}// ModelService