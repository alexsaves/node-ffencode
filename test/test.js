const FFencode = require('../index')
const MakeFakeImageRGBABuffer = require('./lib/utils').MakeFakeImageRGBABuffer
const Jimp = require('jimp');
const fs = require('fs');

var size = {
  width: 600,
  height: 300
};
var fencoder = new FFencode(size.width, size.height, 5, "./out.mp4");
Jimp.read("./test/mouse.png").then(function (mouseimg) {

  Jimp.read("./test/mnts.jpg").then(function (oimg) {
    let W = fencoder.width;
    let H = fencoder.height;
    fencoder.openFrame();
    fencoder.centerRGBAImage(oimg.bitmap.data, oimg.bitmap.width, oimg.bitmap.height);
    fencoder.DrawRGBAImage(mouseimg.bitmap.data, mouseimg.bitmap.width, mouseimg.bitmap.height, 300, 50, 25, 25);
    var frameBuf = fencoder.getPNGOfFrame();
    fs.writeFileSync("./out.png", frameBuf);
    //var frameBuf = fencoder.addRGBABufferFrame(oimg.bitmap.data, oimg.bitmap.width, oimg.bitmap.height);
    fencoder.closeFrame();
    

    fencoder.finalize();
  }).catch(function (err) {
    // handle an exception
    console.log(err);
  });

}).catch(function (err) {
  // handle an exception
  console.log(err);
});