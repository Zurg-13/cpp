import { Component, OnInit, Input } from '@angular/core';
import { Item } from 'src/app/cls/Item';
import { WebsocketService } from 'src/app/srv/websocket.service';
import { Cmnd } from 'src/app/cls/cmnd';
import { CtrlComponent } from '../ctrl.component';
import { CloneVisitor } from '@angular/compiler/src/i18n/i18n_ast';

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

  constructor(private ws: WebsocketService) { 
      this.saveState();
  }

  ngOnInit(): void {

    this.ws.onMessageObserver.subscribe((data: any) => {

      if(this.item.id == data.item.id) {
        
        if(data.err == null) {
          this.saveState();
        }

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
  private saveState(): void { this.prev = Object.assign({}, this.item); }

  // Восстановить состояние. ---------------------------------------------------
  //----------------------------------------------------------------------------
  private restState(): void { this.item = Object.assign({}, this.prev); }

  // Сохранить элемент меню. ---------------------------------------------------
  //----------------------------------------------------------------------------
  public saveItem(): void {
    this.item.type = 0; 
    this.item.room = 0; 

    this.ws.sendMessage(new Cmnd("item_save", this.item));

  }// saveItem

  // Отменить изменения. -------------------------------------------------------
  //----------------------------------------------------------------------------
  public cancelChange(): void {
    this.restState();
  }// cancelChange

}
