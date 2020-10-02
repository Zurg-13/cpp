import { Component, OnInit, Input } from '@angular/core';
import { Item } from 'src/app/cls/item';
import { Attr } from 'src/app/cls/attr';
import { AttrService } from 'src/app/srv/attr.service';
import { WsctService } from 'src/app/srv/wsct.service';
import { ItemService } from 'src/app/srv/item.service';

@Component({
  selector: 'app-attribute',
  templateUrl: './attribute.component.html',
  styleUrls: ['./attribute.component.scss']
})
export class AttributeComponent implements OnInit {

  @Input() attr: Attr;
  @Input() item: Item;

  constructor(
    public item_svc: ItemService
  , public attr_svc: AttrService
  , public wsct_svc: WsctService
  ) {}

  ngOnInit(): void {}

  // Выбрать элемент. ----------------------------------------------------------
  //----------------------------------------------------------------------------
  public sel(): void {
    switch(this.attr_svc.type) {
      case "Type": 
        this.item_svc.updItemType(
          this.item.id, this.attr.id, this.attr.name ); 
        break;

      case "Room": 
        this.item_svc.updItemRoom(
          this.item.id, this.attr.id, this.attr.name ); 
        break;
      
      default: 
          console.error("Неизвестный тип атрибута: " + this.attr_svc.type);

    }// switch(this.attrService.type)

    this.attr_svc.hide();
  }// sel

  // Управление состоянием. ----------------------------------------------------
  //----------------------------------------------------------------------------
  public del(): void { this.attr_svc.drop(this.attr); }
  public add(): void { this.attr_svc.post(this.attr); }
  public upd(): void { this.attr_svc.save(this.attr); }

}// AttributeComponent
