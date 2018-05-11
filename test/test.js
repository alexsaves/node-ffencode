const FFencode = require('../index')
const MakeFakeImageRGBABuffer = require('./lib/utils').MakeFakeImageRGBABuffer

var size = {
  width: 800,
  height: 500
};
var fencoder = new FFencode(size.width, size.height, 5, "./out.mp4");

for (let i = 0; i < 100; i++) {
  fencoder.addRGBABufferFrame(MakeFakeImageRGBABuffer(size.width, size.height));
  console.log(`Added frame ${i}.`);
}

fencoder.finalize();