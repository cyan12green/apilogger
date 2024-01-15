var addon = require('bindings')('addon.node');
var str = "something\0somethingelse\0\0\0sma\0\0";
var enc = addon.EncodeSafe(str)
console.log(str, 'is safely encoded as', addon.EncodeSafe(str))
var dec = addon.DecodeSafe(enc)
console.log('decoded string is', dec)

console.log('checking the truthiness of the encoding/decoding', str === dec)
