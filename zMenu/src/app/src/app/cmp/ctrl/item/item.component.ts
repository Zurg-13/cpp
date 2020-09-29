import { Component, OnInit, Input, ViewChild, ElementRef } from '@angular/core';
import { Item } from 'src/app/cls/Item';
import { WebsocketService } from 'src/app/srv/websocket.service';
import { Cmnd } from 'src/app/cls/cmnd';
import { CtrlComponent } from '../ctrl.component';
import { CloneVisitor } from '@angular/compiler/src/i18n/i18n_ast';
import { ModelService } from 'src/app/srv/model.service';
import { Subscription, interval } from 'rxjs';
import { AttrService } from 'src/app/srv/attr.service';

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
    private ws: WebsocketService
  , public attrService: AttrService  
  , public modelService: ModelService) 
  { 
      this.saveState();
  }

  ngOnInit(): void {
    this.ws.onMessageObserver.subscribe((data: any) => {
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
  } 

  public openAttributeEditor(attributeType: string): void {

    if(this.attributeType == attributeType) { 
        this.attrService.vsbl = !this.attrService.vsbl;
    } else if(this.attrService.vsbl) { 
      this.attrService.hide();
      setTimeout(() => { this.attrService.show(); }, 0);
    } else {
      this.attrService.show();
    }

    this.attributeType = attributeType;
  }

  // Сохранить состояние. ------------------------------------------------------
  //----------------------------------------------------------------------------
  private saveState(): void { 
    this.prev = Object.assign({}, this.item); 
  }

  // Восстановить состояние. ---------------------------------------------------
  //----------------------------------------------------------------------------
  private restState(): void { this.item = Object.assign({}, this.prev); }

  // Сохранить элемент меню. ---------------------------------------------------
  //----------------------------------------------------------------------------
  public saveItem(): void {
    if(this.item.id == null) 
      { this.ws.sendMessage(new Cmnd("item_post", this.item)); }
    else 
      { this.ws.sendMessage(new Cmnd("item_save", this.item)); }
  }// saveItem

  // Отменить изменения. -------------------------------------------------------
  //----------------------------------------------------------------------------
  public cancelChange(): void {
    this.restState();
  }// cancelChange

  // Добавить элемент. ---------------------------------------------------------
  //----------------------------------------------------------------------------
  public newItem(): void {
    let item = new Item(
      null
    , null, null, null, 0, 1
    , this.item.type, this.item.tnme
    , this.item.room, this.item.rnme);
    this.modelService.insItem(this.item.id, item);
  }// addItem

  // Удалить элемент. ----------------------------------------------------------
  //----------------------------------------------------------------------------
  public del() { 
    this.subs = this.subs ? this.subs 
    : this.ws.onMessageObserver.subscribe((data: any) => {
      if("item_drop" == data.cmnd && this.item.id == data.id)
        { this.modelService.delItem(this.item.id); }
    });
    this.ws.sendMessage(new Cmnd("item_drop", this.item)); 
  }// del

}// ItemComponent
