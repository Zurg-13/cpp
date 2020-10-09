import { Item } from './Item';

// Описание структуры Room
export class Room {
    id: number;
    name: string; 
    note: string;
    itm: Item[] = [];

    constructor(id: number, name: string, note: string) {
        this.id = id;
        this.name = name; this.note = note;
    }// constructor

}
