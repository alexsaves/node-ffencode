const FFencode = require('../index')
const MakeFakeImageRGBABuffer = require('./lib/utils').MakeFakeImageRGBABuffer
const Jimp = require('jimp');

var size = {
  width: 200,
  height: 100
};
var fencoder = new FFencode(size.width, size.height, 5, "./out.mp4");

Jimp.read("./test/pencils_small.png").then(function (oimg) {
  let W = fencoder.width;
  let H = fencoder.height;
  var frameBuf = fencoder.addRGBABufferFrame(oimg.bitmap.data, oimg.bitmap.width, oimg.bitmap.height);
  var newImg = new Jimp(W, H, function (err, image) {
    //let frameBuf = oimg.bitmap.data;
    for (let y = 0; y < H; y++) {
      if (y < 3) {
        console.log(y);
      }
      for (let x = 0; x < W; x++) {
        let offset = ((y * W) + x) * 4;
        let r = frameBuf.readUInt8(offset);
        let g = frameBuf.readUInt8(offset + 1);
        let b = frameBuf.readUInt8(offset + 2);
        let a = frameBuf.readUInt8(offset + 3)
        image.setPixelColor(Jimp.rgbaToInt(r, g, b, a), x, y);
      }
    }
    image.write("./out.jpg");
  });
  /*for (let i = 0; i < 5; i++) {
    fencoder.addRGBABufferFrame(MakeFakeImageRGBABuffer(size.width, size.height));
    console.log(`Added frame ${i}.`);
  }*/

  fencoder.finalize();
}).catch(function (err) {
  // handle an exception
  console.log(err);
});