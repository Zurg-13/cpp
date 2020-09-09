import { Attribute } from './attribute';

// Описание структуры Type
export class Type implements Attribute {
    type = 'Type';
    id: number;
    name: string;
    note: string;

    constructor(id: number, name: string, note: string) {
        this.id = id;
        this.name = name;
        this.note = note;
    }
}
