const picjs = require('.')

const pic = new picjs.Picture(800, 600)
pic.setPixel(0, 0, { red: 255, alpha: 127 })

console.log(JSON.stringify(
    pic.getPixel(0, 0)
))