import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';
import { Attribute } from 'src/app/cls/attribute';
import { Type } from 'src/app/cls/type';
import { Item } from 'src/app/cls/item';
import { Room } from 'src/app/cls/room';


@Component({
  selector: 'app-attribute-editor',
  templateUrl: './attribute-editor.component.html',
  styleUrls: ['./attribute-editor.component.scss']
})
export class AttributeEditorComponent implements OnInit {

  @Input() attributeType: string;
  @Input() attributeEditorShow: boolean;
  @Output() attributeEditorShowChange: EventEmitter<any> = new EventEmitter<any>();

  public attribute: Attribute;
  public attributes: Attribute[] = [];

  constructor() { }

  ngOnInit(): void {
    console.log('attributeType: ' + this.attributeType);
    console.log('attributeEditorShow = ' + this.attributeEditorShow);
    switch(this.attributeType) {
      case 'Room':
        this.attribute = new Room(0, null, null);

        // FIXME: Удалить после отладки
        for(let i = 0; i < 5; i++) {
          this.attributes.push(new Room(i, 'room_' + i, null));
        }
        console.log('Тип Room');
        break;

      case 'Type':
        this.attribute = new Type(0, null, null);

        // FIXME: Удалить после отладки
        for(let i = 0; i < 5; i++) {
          this.attributes.push(new Type(i, 'type_' + i, null));
        }
        console.log('Тип Type');
        console.log(this.attributes);
        break;

      default:
        console.log('Неизвестный тип атрибута');
    }

    
  }

  public addAttribute(): void {
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
     this.attributes.push(newAttribute);
  }

  public close(): void {
    this.attributeEditorShow = false;
    this.attributeEditorShowChange.emit(this.attributeEditorShow);
    console.log('close.');
  }

}
