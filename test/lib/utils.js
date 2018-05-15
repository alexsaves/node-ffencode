const fs = require('fs');
const zlib = require('zlib');
/**
 * Build a fake image buffer
 * @param {Number} width 
 * @param {Number} height 
 */
const MakeFakeImageRGBABuffer = function(width, height) {
  const pixlen = width * height;
  const arr = new Uint8Array(pixlen * 4);
  for (let i = 0; i < pixlen; i++) {
    arr[(i * 4)] = Math.round((Math.random() * 100) + 155);
    arr[(i * 4) + 1] = Math.round((Math.random() * 100) + 155);
    arr[(i * 4) + 2] = Math.round((Math.random() * 100) + 155);
    arr[(i * 4) + 3] = 255;
  }
  return Buffer.from(arr);
};

/**
 * Write a test image to a file
 * @param {String} iname 
 * @param {Buffer} buf 
 * @param {Number} width 
 * @param {Number} height 
 */
const WriteImageToFile = function(iname, buf, width, height) {
  buf = zlib.deflateRawSync(buf);
  var fname = iname.toLowerCase().trim() + "_" + width + "_" + height + ".df";
  fs.writeFileSync("./test/" + fname, buf);
};

/**
 * Read a compressed RGBA image from a prepared file
 * @param {String} fname The filename
 */
const ReadImageFromFile = function(fname) {
  var buf = fs.readFileSync(fname);
  buf = zlib.inflateRawSync(buf);
  var bits = fname.substr(0, fname.lastIndexOf('.')).split('_');
  var width = parseInt(bits[1]);
  var height = parseInt(bits[2]);
  return {
    rgba: buf,
    width: width,
    height: height
  };
};

// Export it
module.exports = {
  MakeFakeImageRGBABuffer,
  WriteImageToFile,
  ReadImageFromFile
};