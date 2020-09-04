import { Component, OnInit, Input } from '@angular/core';
import { Attribute } from 'src/app/cls/attribute';

@Component({
  selector: 'app-attribute',
  templateUrl: './attribute.component.html',
  styleUrls: ['./attribute.component.scss']
})
export class AttributeComponent implements OnInit {

  @Input() attribute: Attribute;

  constructor() { }

  ngOnInit(): void {
  }

}
