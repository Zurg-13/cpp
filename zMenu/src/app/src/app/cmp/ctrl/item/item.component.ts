import { Component, OnInit, Input, ViewChild, ElementRef } from '@angular/core';
import { Item } from 'src/app/cls/Item';
import { Cmnd } from 'src/app/cls/cmnd';
import { Subscription, interval } from 'rxjs';
import { AttrService } from 'src/app/srv/attr.service';
import { ItemService } from 'src/app/srv/item.service';
import { WsctService } from 'src/app/srv/wsct.service';

@Component({
  selector: 'app-item',
  templateUrl: './item.component.html',
  styleUrls: ['./item.component.scss']
})
export class ItemComponent implements OnInit {

  @ViewChild('name') 
  nameInput: ElementRef;

  @Input() 
  item: Item;

  private subs: Subscription;  
  private prev: Item;

  public attributeEditorShow: boolean = false;
  public attributeType: string;

  constructor(
    public wsct_svc: WsctService
  , public attr_svc: AttrService  
  , public item_svc: ItemService) 
    {}

  ngOnInit(): void {
    this.wsct_svc.onMessageObserver.subscribe((data: any) => {
      switch(data.cmnd) {
        case "item_save":
          if(this.item.id == data.id) 
            { if(data.err == null) { this.saveState(); }}
        break;
         
        case "item_post":
          if(this.item.id == null) 
            { if(data.err == null) { this.item.id = data.id; }}
        break;
      }
    });
  }

  ngAfterViewInit() {
    if(this.item.id == null) 
      { setTimeout(() => { this.nameInput.nativeElement.focus(); }, 0) }      
    else
      { this.saveState(); }
  } 

  public openAttributeEditor(attributeType: string): void {

    if(this.attributeType == attributeType) { 
        this.attr_svc.vsbl = !this.attr_svc.vsbl;
    } else if(this.attr_svc.vsbl) { 
      this.attr_svc.hide();
      setTimeout(() => { this.attr_svc.show(); }, 0);
    } else {
      this.attr_svc.show();
    }

    this.attributeType = attributeType;
  }

  // Сохранить состояние. ------------------------------------------------------
  //----------------------------------------------------------------------------
  private saveState(): void {  this.prev = Object.assign({}, this.item); }

  // Восстановить состояние. ---------------------------------------------------
  //----------------------------------------------------------------------------
  private restState(): void { this.item = Object.assign({}, this.prev); }

  // Проверка на наличие изменений. --------------------------------------------
  //----------------------------------------------------------------------------
  public isChanged(): boolean { 
      let tmp = !( 
         this.prev != undefined && this.item != undefined
      && this.prev.name === this.item.name
      && this.prev.note === this.item.note
      && this.prev.cost === this.item.cost
      && this.prev.type === this.item.type
      && this.prev.room === this.item.room);

      return tmp;
  }

  // Сохранить элемент меню. ---------------------------------------------------
  //----------------------------------------------------------------------------
  public saveItem(): void {
    if(this.item.id == null) 
      { this.wsct_svc.send(new Cmnd("item_post", this.item)); }
    else 
      { this.wsct_svc.send(new Cmnd("item_save", this.item)); }

    this.attr_svc.hide();
  }// saveItem

  // Отменить изменения. -------------------------------------------------------
  //----------------------------------------------------------------------------
  public cancelChange(): void { this.restState(); this.attr_svc.hide(); }

  // Добавить элемент. ---------------------------------------------------------
  //----------------------------------------------------------------------------
  public newItem(): void {
    let item = new Item(
      null
    , null, null, null, 0, 1
    , this.item.type, this.item.tnme
    , this.item.room, this.item.rnme );

    this.item_svc.insItem(this.item.id, item);
  }// addItem

  // Удалить элемент. ----------------------------------------------------------
  //----------------------------------------------------------------------------
  public del() { 
    this.subs = this.subs ? this.subs 
    : this.wsct_svc.onMessageObserver.subscribe((data: any) => {
      if("item_drop" == data.cmnd && this.item.id == data.id)
        { this.item_svc.delItem(this.item.id); }
    });
    this.wsct_svc.send(new Cmnd("item_drop", this.item)); 
  }// del

}// ItemComponent
