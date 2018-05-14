const FFencode = require('../index')
const MakeFakeImageRGBABuffer = require('./lib/utils').MakeFakeImageRGBABuffer
const Jimp = require('jimp');
const fs = require('fs');

var size = {
  width: 1000,
  height: 500
};
var fencoder = new FFencode(size.width, size.height, 5, "./out.mp4");
Jimp.read("./test/mouse.png").then(function (mouseimg) {

  Jimp.read("./test/pencils_small.png").then(function (oimg) {
    let W = fencoder.width;
    let H = fencoder.height;
    for (var i = 0; i < 10; i++) {
    //while (true) {
      var nw = new Date();
      fencoder.openFrame();
      fencoder.centerRGBAImage(oimg.bitmap.data, oimg.bitmap.width, oimg.bitmap.height);
      fencoder.DrawRGBAImage(mouseimg.bitmap.data, mouseimg.bitmap.width, mouseimg.bitmap.height, 280, 50, 50, 50);
      //var frameBuf = fencoder.getPNGOfFrame();
      fencoder.closeFrame();
      console.log("FRAME TIME: ", (new Date()) - nw);
      //fs.writeFileSync("./out_" + i + ".png", frameBuf);
    }

    fencoder.finalize();
  }).catch(function (err) {
    // handle an exception
    console.log(err);
  });

}).catch(function (err) {
  // handle an exception
  console.log(err);
});