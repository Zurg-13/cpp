import { Component, OnInit, Input } from '@angular/core';
import { ModelService } from 'src/app/srv/model.service';
import { Item } from 'src/app/cls/item';
import { Attr } from 'src/app/cls/attr';
import { AttrService } from 'src/app/srv/attr.service';

@Component({
  selector: 'app-attribute',
  templateUrl: './attribute.component.html',
  styleUrls: ['./attribute.component.scss']
})
export class AttributeComponent implements OnInit {

  @Input() attr: Attr;
  @Input() item: Item;

  constructor(
    public modelService: ModelService
  , public attrService: AttrService
  ) {}

  ngOnInit(): void {}

  public sel(): void {
    switch(this.attrService.type) {
      case "Type": 
        this.modelService.updItemType(
          this.item.id, this.attr.id, this.attr.name ); 
        break;

      case "Room": 
        this.modelService.updItemRoom(
          this.item.id, this.attr.id, this.attr.name ); 
        break;
      
      default: 
          console.error("Неизвестный тип атрибута: " + this.attrService.type);

    }

    this.attrService.hide();

  }

  public del(): void {

  }

  public new(): void {

  }

  public upd(): void {
    
  }


}
