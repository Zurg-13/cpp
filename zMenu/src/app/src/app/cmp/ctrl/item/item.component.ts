import { Component, OnInit, Input } from '@angular/core';
import { Item } from 'src/app/cls/Item';
import { WebsocketService } from 'src/app/srv/websocket.service';
import { Cmnd } from 'src/app/cls/cmnd';
import { CtrlComponent } from '../ctrl.component';
import { CloneVisitor } from '@angular/compiler/src/i18n/i18n_ast';
import { ModelService } from 'src/app/srv/model.service';

@Component({
  selector: 'app-item',
  templateUrl: './item.component.html',
  styleUrls: ['./item.component.scss']
})
export class ItemComponent implements OnInit {

  @Input() item: Item;

  private prev: Item;

  public attributeEditorShow: boolean = false;
  public attributeType: string;

  constructor(
    private ws: WebsocketService
  , public modelService: ModelService) 
  { 
      this.saveState();
  }

  ngOnInit(): void {

    this.ws.onMessageObserver.subscribe((data: any) => {


      switch(data["cmnd"]) {
        case "item_save":
          if(this.item.id == data.item.id) 
            { if(data.err == null) { this.saveState(); }}
        break;
         
        case "item_post":
          if(this.item.id == null) 
            { if(data.err == null) { this.item.id = data.item.id; }}
        break;
      }

    });

  }

  public openAttributeEditor(attributeType: string): void {
    console.log(attributeType);
    this.attributeType = attributeType;
    this.attributeEditorShow = true;
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
      this.modelService.insItem(
        this.item.id
      , new Item(
          null
        , null, null, null
        , 0, 1, this.item.type, this.item.room) );
  }// addItem

}// ItemComponent
