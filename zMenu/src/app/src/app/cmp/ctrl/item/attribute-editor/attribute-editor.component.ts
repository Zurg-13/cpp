import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';

import { Item } from 'src/app/cls/item';
import { Attr } from 'src/app/cls/attr';
import { Cmnd } from 'src/app/cls/cmnd';
import { Subscription } from 'rxjs';
import { AttrService } from 'src/app/srv/attr.service';
import { WsctService } from 'src/app/srv/wsct.service';

@Component({
  selector: 'app-attribute-editor',
  templateUrl: './attribute-editor.component.html',
  styleUrls: ['./attribute-editor.component.scss']
})
export class AttributeEditorComponent implements OnInit {

  @Input() attributeType: string;
  @Input() item: Item;

  private r_sb: Subscription;  
  private t_sb: Subscription;  

  // Конструктор. --------------------------------------------------------------
  //----------------------------------------------------------------------------
  constructor(
    private wsct_svc: WsctService
  , public  attr_svc: AttrService
  ) {}

  // Выполняется при инициализации. --------------------------------------------
  //----------------------------------------------------------------------------
  ngOnInit(): void {

    switch(this.attributeType) {

      case 'Room':
        this.wsct_svc.send(new Cmnd("room_list", null));
        this.r_sb = this.r_sb ? this.r_sb
        : this.wsct_svc.onMessageObserver.subscribe((data: any) => {
          if("room_list" == data.cmnd) 
            { this.attr_svc.setList(data.list, this.attributeType); } 
        });
        break;

      case 'Type':
        this.wsct_svc.send(new Cmnd("type_list", null));
        this.t_sb = this.t_sb ? this.t_sb
        : this.wsct_svc.onMessageObserver.subscribe((data: any) => {
          if("type_list" == data.cmnd) 
            { this.attr_svc.setList(data.list, this.attributeType); }          
        });
        break;

      default:
        console.log('Неизвестный тип атрибута');
    }
  }// ngOnInit

/*  
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
*/  

  public hide(): void { this.attr_svc.hide(); }

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
