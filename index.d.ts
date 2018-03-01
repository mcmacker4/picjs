
declare namespace PicJS {

    export class Picture {

        readonly width: Number
        readonly height: Number
        
        constructor(width: Number, height: Number)

        getPixel(x: Number, y: Number) : Number
        setPixel(x: Number, y: Number, color: Number)

    }

}

export = PicJS