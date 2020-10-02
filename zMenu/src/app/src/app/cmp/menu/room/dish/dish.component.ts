import { Component, OnInit, Input } from '@angular/core';
import { Item } from 'src/app/cls/Item';

@Component({
  selector: 'app-dish',
  templateUrl: './dish.component.html',
  styleUrls: ['./dish.component.scss']
})
export class DishComponent implements OnInit {

  @Input()
  dish: Item;

  constructor() { }

  ngOnInit(): void {
  }

}
