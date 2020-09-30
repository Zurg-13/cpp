import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';

import { Item } from 'src/app/cls/item';
import { Attr } from 'src/app/cls/attr';
import { WebsocketService } from 'src/app/srv/websocket.service';
import { Cmnd } from 'src/app/cls/cmnd';
import { Subscription } from 'rxjs';
import { AttrService } from 'src/app/srv/attr.service';


@Component({
  selector: 'app-attribute-editor',
  templateUrl: './attribute-editor.component.html',
  styleUrls: ['./attribute-editor.component.scss']
})
export class AttributeEditorComponent implements OnInit {


  @Input() attributeType: string;
/*  
  @Input() attributeEditorShow: boolean;
*/  

  @Input() item: Item;

/*  
  @Output() attributeEditorShowChange: EventEmitter<any> = new EventEmitter<any>();
  @Output() itemChange: EventEmitter<any> = new EventEmitter<any>();
  public atr: Attribute[] = [];
*/  

  private r_sb: Subscription;  
  private t_sb: Subscription;  

  constructor(
    private ws: WebsocketService
  , public  attrService: AttrService
  ) {}

  ngOnInit(): void {

    switch(this.attributeType) {

      case 'Room':
        this.ws.sendMessage(new Cmnd("room_list", null));
        this.r_sb = this.r_sb ? this.r_sb
        : this.ws.onMessageObserver.subscribe((data: any) => {
          if("room_list" == data.cmnd) 
            { this.attrService.setList(data.list, this.attributeType); } 
        });
        break;

      case 'Type':
        this.ws.sendMessage(new Cmnd("type_list", null));
        this.t_sb = this.t_sb ? this.t_sb
        : this.ws.onMessageObserver.subscribe((data: any) => {
          if("type_list" == data.cmnd) 
            { this.attrService.setList(data.list, this.attributeType); }          
        });
        break;

      default:
        console.log('Неизвестный тип атрибута');
    }
  }

  public addAttr(): void {
    let newAttribute: Attr;
    switch(this.attributeType) {
      case 'Room':
        newAttribute = new Attr(null, null, null, "Room");
        break;

      case 'Type':
        newAttribute = new Attr(0, null, null, "Type");
        break;

      default:
        console.log('Неизвестный тип атрибута');
    }

    this.attrService.addElem(newAttribute);
  }

  public hide(): void { this.attrService.hide(); }

  public sel(attr: Attr): void {
    switch(this.attributeType) {
      case "Type": 
        this.item.type = attr.id; 
        this.item.tnme = attr.name;
        break;

      case "Room": 
        this.item.room = attr.id; 
        this.item.rnme = attr.name;
        break;
    }

    this.hide();
  }
}
