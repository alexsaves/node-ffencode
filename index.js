const pjson = require('./package.json');
const shortid = require('shortid');
const fs = require('fs');
var FFEncoder;
if (fs.existsSync('./build/Debug/ffencode.node')) {
  FFEncoder = require('./build/Debug/ffencode.node').FFEncoder
} else {
  FFEncoder = require('./build/Release/ffencode.node').FFEncoder;
}

/**
 * Manages encoding to FFMpeg
 */
class FFEncode {
  /**
   * 
   * @param {Number} width 
   * @param {Number} height 
   * @param {Number} fps 
   */
  constructor(width, height, fps, filename) {
    if (isNaN(width)) {
      throw new Error("Invalid width parameter.");
    }
    if (isNaN(height)) {
      throw new Error("Invalid height parameter.");
    }
    if (isNaN(fps)) {
      throw new Error("Invalid fps parameter.");
    }
    this.width = Math.min(99999, Math.max(1, Math.round(width)));
    this.height = Math.min(99999, Math.max(1, Math.round(height)));
    this.fps = Math.max(1, Math.min(120, Math.round(fps)));
    this.filename = filename || (shortid.generate() + ".mp4");
    this._enc = new FFEncoder(this.width, this.height, this.fps, this.filename);
  }

  openFrame() {
    this._enc.openFrame();
  }

  closeFrame() {
    this._enc.closeFrame();
  }

  /**
   * Add a frame for the video
   * @param {Buffer} buf RGBA Pixel buffer
   * @param {Number} width Width of image (must be <= movie width)
   * @param {Number} height Height of image (must be <= movie height)
   */
  centerRGBAImage(buf, width, height) {
    if (!(buf instanceof Buffer)) {
      throw new Error("Argument must be a buffer.");
    }
    if (isNaN(width) || isNaN(height)) {
      width = this.width;
      height = this.height;
    }
    if (width * height * 4 != buf.length) {
      throw new Error("Buffer length does not match provided width and height * 4 (not an RGBA array?)");
    }
    var nw = new Date();
    this._enc.centerRGBAImage(buf, width, height);
    console.log("TIME:", (new Date()) - nw);
  }

  getPNGOfFrame() {
    return this._enc.getPNGOfFrame();
  }

  DrawRGBAImage(buf, width, height, x, y, target_width, target_height) {
    if (!(buf instanceof Buffer)) {
      throw new Error("Argument must be a buffer.");
    }
    if (isNaN(width) || isNaN(height)) {
      width = this.width;
      height = this.height;
    }
    if (width * height * 4 != buf.length) {
      throw new Error("Buffer length does not match provided width and height * 4 (not an RGBA array?)");
    }
    this._enc.drawRGBAImage(buf, width, height, x, y, target_width, target_height);
  }

  /**
   * Finish the movie off
   */
  finalize() {
    delete this._enc;
  }
};

// Export it
module.exports = FFEncode;