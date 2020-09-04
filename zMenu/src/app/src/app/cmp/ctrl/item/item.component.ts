import { Component, OnInit, Input } from '@angular/core';
import { Item } from 'src/app/cls/Item';

@Component({
  selector: 'app-item',
  templateUrl: './item.component.html',
  styleUrls: ['./item.component.scss']
})
export class ItemComponent implements OnInit {

  @Input() item: Item;

  public attributeEditorShow: boolean = false;
  public attributeType: string;

  constructor() { 

  }

  ngOnInit(): void {
  }

  public openAttributeEditor(attributeType: string): void {
    console.log(attributeType);
    this.attributeType = attributeType;
    this.attributeEditorShow = true;
  }

}
