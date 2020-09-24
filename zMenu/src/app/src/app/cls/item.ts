
// Описание структуры Item
export class Item {
    id: number;
    name: string;
    note: string;
    cost: number;
    stat: number;
    hide: number;
    type: number;
    room: number;

    constructor(
        id: number
      , name: string
      , note: string
      , cost: number
      , stat: number
      , hide: number
      , type: number
      , room: number) 
    {
        this.id = id;
        this.name = name;
        this.note = note;
        this.cost = cost;
        this.stat = stat;
        this.hide = hide;
        this.type = type;
        this.room = room;
    }// constructor
}
