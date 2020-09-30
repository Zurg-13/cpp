import { Injectable } from '@angular/core';
import {Attr} from '../cls/attr'

@Injectable({ providedIn: 'root' })

export class AttrService {
  private list: Attr[] = []; // Список редактируемых карточек attr

  public slct: Attr;
  public vsbl = false;
  public type: string;

  public getList(): Attr[] { return this.list; }
  public setList(list: Attr[], type: string) 
    { this.list = list; this.type = type; }

  public setElem(elem: Attr) { this.list.push(elem); }
  public getElem(id: number): Attr { 
    for(let elem of this.list) { if(elem.id == id) { return elem; }}
    return null;
  }// getItem

  public insElem(id: number, elem: Attr) {
    for(let i=0; i<this.list.length; i++) 
      { if(this.list[i].id == id) { this.list.splice(i, 0, elem); return; }}
  }// insItem

  public delElem(id: number) {
    for(let i=0; i<this.list.length; i++) 
      { if(this.list[i].id == id) { this.list.splice(i, 1); return; }}    
  }// delItem

  public addElem(elem: Attr) { this.list.push(elem); }

  public hide() { this.vsbl = false; }
  public show() { this.vsbl = true; }

  constructor() { }

}// AttrService
