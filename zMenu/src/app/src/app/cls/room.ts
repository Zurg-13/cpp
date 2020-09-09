import { Attribute } from './attribute';

// Описание структуры Room
export class Room implements Attribute {

    type = 'Room';
    id: number;
    name: string;
    note: string;

    constructor(id: number, name: string, note: string) {
        this.id = id;
        this.name = name;
        this.note = note;
    }
}
