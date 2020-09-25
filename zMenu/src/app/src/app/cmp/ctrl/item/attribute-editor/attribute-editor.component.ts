import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';
import { Attribute } from 'src/app/cls/attribute';
import { Type } from 'src/app/cls/type';
import { Item } from 'src/app/cls/item';
import { Room } from 'src/app/cls/room';
import { WebsocketService } from 'src/app/srv/websocket.service';
import { Cmnd } from 'src/app/cls/cmnd';
import { Subscription } from 'rxjs';


@Component({
  selector: 'app-attribute-editor',
  templateUrl: './attribute-editor.component.html',
  styleUrls: ['./attribute-editor.component.scss']
})
export class AttributeEditorComponent implements OnInit {

  @Input() attributeType: string;
  @Input() attributeEditorShow: boolean;
  @Input() item: Item;

  @Output() attributeEditorShowChange: EventEmitter<any> = new EventEmitter<any>();
  @Output() itemChange: EventEmitter<any> = new EventEmitter<any>();

  public atr: Attribute[] = [];

  private r_sb: Subscription;  
  private t_sb: Subscription;  

  constructor(private ws: WebsocketService) { }

  ngOnInit(): void {
    console.log('attributeType: ' + this.attributeType);
    console.log('attributeEditorShow = ' + this.attributeEditorShow);
    switch(this.attributeType) {

      case 'Room':
        this.ws.sendMessage(new Cmnd("room_list", null));
        this.r_sb = this.r_sb ? this.r_sb
        : this.ws.onMessageObserver.subscribe((data: any) => {
          if("room_list" == data.cmnd) 
            { this.atr = data.list; } 

        });
        break;

      case 'Type':
        this.ws.sendMessage(new Cmnd("type_list", null));
        this.t_sb = this.t_sb ? this.t_sb
        : this.ws.onMessageObserver.subscribe((data: any) => {
          if("type_list" == data.cmnd) 
            { this.atr = data.list; }          
        });
        break;

      default:
        console.log('Неизвестный тип атрибута');
    }
  }

  public addAttr(): void {
    let newAttribute: Attribute = null;
    switch(this.attributeType) {
      case 'Room':
        newAttribute = new Room(0, null, null);
        console.log('Тип Room');
        break;

      case 'Type':
        newAttribute = new Type(0, null, null);
        break;

      default:
        console.log('Неизвестный тип атрибута');
    }
     this.atr.push(newAttribute);
  }

  public close(): void {
    this.attributeEditorShow = false;
    this.attributeEditorShowChange.emit(this.attributeEditorShow);
  }

  public sel(attr: Attribute): void {
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
    this.itemChange.emit(this.item);
    this.close();
  }
}
