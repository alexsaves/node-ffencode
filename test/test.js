const FFencode = require('../index');
const MakeFakeImageRGBABuffer = require('./lib/utils').MakeFakeImageRGBABuffer;
const WriteImageToFile = require('./lib/utils').WriteImageToFile;
const ReadImageFromFile = require('./lib/utils').ReadImageFromFile;
const fs = require('fs');

const video_size = {
  width: 1000,
  height: 700
};

const fencoder = new FFencode(video_size.width, video_size.height, 5, "./out.mp4");
const mouseImg = ReadImageFromFile("./test/mouse_98_98.df");
const mountainImg = ReadImageFromFile("./test/mountain_1600_640.df");
const pencilsImg = ReadImageFromFile("./test/pencils_1024_922.df");
const balloonImg = ReadImageFromFile("./test/balloon_250_167.df");

for (var i = 0; i < 10; i++) {
  var nw = new Date();
  fencoder.openFrame();
  fencoder.centerRGBAImage(mountainImg.rgba, mountainImg.width, mountainImg.height);
  fencoder.centerRGBAImage(balloonImg.rgba, balloonImg.width, balloonImg.height);
  fencoder.centerRGBAImage(pencilsImg.rgba, pencilsImg.width, pencilsImg.height);
  fencoder.drawRGBAImage(mouseImg.rgba, mouseImg.width, mouseImg.height, 280, 50, 50, 50, 0.5);
  //let frameBuf = fencoder.getRGBABufferOfFrame();
  //let frameBuf = fencoder.getPNGOfFrame();
  let frameBuf = fencoder.getJPGOfFrame(5);
  //let frameBuf;
  fs.writeFileSync("./out_" + i + ".jpg", frameBuf);  
  fencoder.closeFrame();
  console.log("Bitmap Len: " + (frameBuf && frameBuf.length) + ", FRAME TIME: ", (new Date()) - nw);
}
console.log("About to finalize");
fencoder.finalize();
console.log("DID finalize");