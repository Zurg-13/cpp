import { Injectable } from '@angular/core';
import { Item } from '../cls/Item';

@Injectable({ providedIn: 'root' })

// Модель ленты элементов меню. -----------------------------------------------
//-----------------------------------------------------------------------------
export class ModelService {

  private list: Item[] = []; // Список редактируемых карточек item
  public item_slct: Item;

  public getList(): Item[] { return this.list; }
  public setList(list: Item[]) { this.list = list; }

  public setItem(item: Item) { this.list.push(item); }
  public getItem(id: number): Item { 
    for(let item of this.list) { if(item.id == id) { return item; }}
    return null;
  }// getItem

  public insItem(id: number, item: Item) {
    for(let i=0; i<this.list.length; i++) 
      { if(this.list[i].id == id) { this.list.splice(i, 0, item); return; }}
  }// insItem

  public delItem(id: number) {
    for(let i=0; i<this.list.length; i++) 
      { if(this.list[i].id == id) { this.list.splice(i, 1); return; }}    
  }// delItem

  public updItemRoom(id: number, r_id: number, name: string) {
      let item = this.getItem(id);
      item.room = r_id; item.rnme = name;

      console.log(this.list);
  }

  public updItemType(id: number, t_id: number, name: string) {
      let item = this.getItem(id);
      item.type = t_id; item.tnme = name;
  }

  constructor() { 
  }// constructor

}// ModelService