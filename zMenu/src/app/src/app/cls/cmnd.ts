// Класс команды. --------------------------------------------------------------
//------------------------------------------------------------------------------
export class Cmnd {
    cmnd: string;
    data: any;

    constructor(cmnd: string, data?: any ) {
        this.cmnd = cmnd; this.data = data;
    }
}// Cmnd