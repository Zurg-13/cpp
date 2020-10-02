import { Injectable } from '@angular/core';
import {Attr} from '../cls/attr'
import { Cmnd } from '../cls/cmnd';
import { WsctService } from './wsct.service';

@Injectable({ providedIn: 'root' })

/* Обслуживание редактора атрибутов. ******************************************/
/******************************************************************************/
export class AttrService {

  private list: Attr[] = []; // Список редактируемых карточек attr
  private cntr = -1;

  public slct: Attr;
  public vsbl = false;
  public type: string;

  // Конструктор. --------------------------------------------------------------
  //----------------------------------------------------------------------------
  constructor(
    private wsct_svc: WsctService
  ) {
    this.wsct_svc.onMessageObserver.subscribe((data: any) => {
      switch(data.cmnd) {

        case "type_post":
        case "room_post": {
          let elem = this.getElem(data.id);
          if(elem) { elem.id = data.sign; }
        } break;

        case "type_save":
        case "room_save": {
          let elem = this.getElem(data.id);
          if(elem) { console.log("!!! СОХРАНИТЬ СОСТОЯНИЕ !!!"); }
        } break;

        case "type_drop":
        case "room_drop": {
          let elem = this.getElem(data.id);
          if(elem) { this.delElem(elem.id); }
        } break;

      }// switch(data.cmnd)
    });
  }// constructor

  // WS: Сбросить элемент. -----------------------------------------------------
  //----------------------------------------------------------------------------
  public drop(attr: Attr) { this.wsct_svc.send(this.CMND("drop", attr)); }

  // WS: Сохранить состояние. --------------------------------------------------
  //----------------------------------------------------------------------------
  public save(attr: Attr) { this.wsct_svc.send(this.CMND("save", attr)); }

  // WS: Выделить и опубликовать новый элемент. -------------------------------- 
  //---------------------------------------------------------------------------- 
  public post(prnt: Attr) {
    let attr = new Attr(this.cntr--, "", "", prnt.type);
    this.insElem(prnt.id, attr); 
    this.wsct_svc.send(this.CMND("post", attr));
  }// post

  // Сгененрировать команду. ---------------------------------------------------
  //----------------------------------------------------------------------------
  private CMND(cnme: string, attr: Attr): Cmnd {
    return new Cmnd(
    ( this.type == "Type" ? "type"
    : this.type == "Room" ? "room"
    : "## ERR ##") + "_" + cnme
    , attr );
  }// CMND

  // Список атрибутов. ---------------------------------------------------------
  //----------------------------------------------------------------------------
  public getList(): Attr[] { return this.list; }
  public setList(list: Attr[], type: string) 
    { this.list = list; this.type = type; }

  // Элемент списка атрибутов. -------------------------------------------------
  //----------------------------------------------------------------------------
  public getElem(id: number): Attr { 
    for(let elem of this.list) { if(elem.id == id) { return elem; }}
    return null;
  }// getItem

  // Видимость редактора. ------------------------------------------------------
  //----------------------------------------------------------------------------
  public hide() { this.vsbl = false; }
  public show() { this.vsbl = true; }

  // Удалить элемент. ----------------------------------------------------------
  //----------------------------------------------------------------------------
  private delElem(id: number) {
    for(let i=0; i<this.list.length; i++) 
      { if(this.list[i].id == id) { this.list.splice(i, 1); return; }}    
  }// delItem

  // Вставить элемент. ---------------------------------------------------------
  //----------------------------------------------------------------------------
  private insElem(id: number, elem: Attr) {
    for(let i=0; i<this.list.length; i++) 
      { if(this.list[i].id == id) { this.list.splice(i, 0, elem); return; }}
  }// insItem

}// AttrService
