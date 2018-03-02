
declare namespace PicJS {

    export enum Interpolation {
        NONE = 0,
        LINEAR,
        QUADRATIC
    }

    export interface Pixel {
        red?: Number
        green?: Number
        blue?: Number
        alpha?: Number

        uintValue?: Number
    }

    export class Picture {

        readonly width: Number
        readonly height: Number
        
        constructor(width: Number, height: Number)

        getPixel(x: Number, y: Number) : Pixel
        setPixel(x: Number, y: Number, data: Pixel)

        resize(newWidth: Number, newHeight: Number)
        scale(newWidth: Number, newHeight: Number, interpolate: Interpolation)

        paste(picture: Picture, x: Number, y: Number)

        static New(width: Number, height: Number)

    }

}

export = PicJS