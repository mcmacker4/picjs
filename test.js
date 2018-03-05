const picjs = require('.')
const assert = require('assert')

console.log("Creating picture.")
const pic = new picjs.Picture(800, 600)

console.log("Checking setPixel/getPixel")
pic.setPixel(0, 0, { red: 255, alpha: 127 })

const pixel = pic.getPixel(0, 0)
assert.equal(pixel.red, 255)
assert.equal(pixel.green, 0)
assert.equal(pixel.blue, 0)
assert.equal(pixel.alpha, 127)
console.log("OK")

//Need some way to check if the picture buffer
//is the correct size after resizing.
//Maybe use a std::array instead of c-style array
console.log("Testing resize")
pic.resize(80, 60)
assert.equal(pic.width, 80)
assert.equal(pic.height, 60)
console.log("OK")