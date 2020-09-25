import { Component, OnInit, Input } from '@angular/core';
import { Attribute } from 'src/app/cls/attribute';
import { ModelService } from 'src/app/srv/model.service';
import { Item } from 'src/app/cls/Item';

@Component({
  selector: 'app-attribute',
  templateUrl: './attribute.component.html',
  styleUrls: ['./attribute.component.scss']
})
export class AttributeComponent implements OnInit {

  @Input() attr: Attribute;
  @Input() type: string;
  @Input() item: Item;

  constructor(
    public modelService: ModelService
  ) { }

  ngOnInit(): void {
    
  }

  public sel(): void {

    switch(this.type) {
      case "Type": 
        this.modelService.updItemType(
          this.item.id, this.attr.id, this.attr.name ); 
        break;

      case "Room": 
        this.modelService.updItemRoom(
          this.item.id, this.attr.id, this.attr.name ); 
        break;
    }

  }


}
