
// Описание структуры Item
export class Item {
    id: number;
    name: string; note: string;
    cost: number;
    stat: number; hide: number;
    type: number; tnme: string;
    room: number; rnme: string;

    constructor(
        id: number
      , name: string, note: string
      , cost: number
      , stat: number, hide: number
      , type: number, tnme: string
      , room: number, rnme: string ) 
    {
        this.id = id;
        this.name = name; this.note = note;
        this.cost = cost;
        this.stat = stat; this.hide = hide;
        this.type = type; this.tnme = tnme;
        this.room = room; this.rnme = rnme;
    }// constructor
}
