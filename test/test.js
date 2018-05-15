const FFencode = require('../index');
const MakeFakeImageRGBABuffer = require('./lib/utils').MakeFakeImageRGBABuffer;
const WriteImageToFile = require('./lib/utils').WriteImageToFile;
const ReadImageFromFile = require('./lib/utils').ReadImageFromFile;
const fs = require('fs');

const video_size = {
  width: 500,
  height: 200
};

const fencoder = new FFencode(video_size.width, video_size.height, 5, "./out.mp4");
const mouseImg = ReadImageFromFile("./test/mouse_98_98.df");
const mountainImg = ReadImageFromFile("./test/mountain_1600_640.df");
const pencilsImg = ReadImageFromFile("./test/pencils_1024_922.df");
const balloonImg = ReadImageFromFile("./test/balloon_250_167.df");

for (var i = 0; i < 10; i++) {
  //while (true) {
  var nw = new Date();
  fencoder.openFrame();
  fencoder.centerRGBAImage(mountainImg.rgba, mountainImg.width, mountainImg.height);
  fencoder.centerRGBAImage(balloonImg.rgba, balloonImg.width, balloonImg.height);
  fencoder.centerRGBAImage(pencilsImg.rgba, pencilsImg.width, pencilsImg.height);  
  fencoder.drawRGBAImage(mouseImg.rgba, mouseImg.width, mouseImg.height, 280, 50, 50, 50, 0.5);
  //var frameBuf = fencoder.getPNGOfFrame();
  fencoder.closeFrame();
  console.log("FRAME TIME: ", (new Date()) - nw);
  //fs.writeFileSync("./out_" + i + ".png", frameBuf);
}

fencoder.finalize();