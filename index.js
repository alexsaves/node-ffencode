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
    this._frameOpen = false;
  }

  /**
   * Start a new frame for rendering
   */
  openFrame() {
    if (this._frameOpen) {
      throw new Error("Frame was already open.");
    }
    this._frameOpen = true;
    this._enc.openFrame();
  }

  /**
   * Shut down the frame and release memory
   */
  closeFrame() {
    if (!this._frameOpen) {
      throw new Error("Frame was already closed.");
    }
    this._frameOpen = false;
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
    
    this._enc.centerRGBAImage(buf, width, height);
    
  }

  /**
   * Get a PNG of the current frame
   */
  getPNGOfFrame() {
    return this._enc.getPNGOfFrame();
  }

  /**
   * Get a PNG of the current frame
   */
  getRGBABufferOfFrame() {
    return this._enc.getBufferOfFrame();
  }

  /**
   * Draw a sprite onto the frame
   * @param {Buffer} buf RGBA Buffer of unsigned ints 
   * @param {Number} width Width of the source image
   * @param {Number} height Height of the source image
   * @param {Number} x Where to place the image onto the canvas
   * @param {Number} y Where to place the image onto the canvas
   * @param {Number} target_width How wide to make the image on the canvas
   * @param {Number} target_height How tall to make the image on the canvas
   * @param {Number} opacity (0-1) How transparent (1 == opaque)
   */
  drawRGBAImage(buf, width, height, x, y, target_width, target_height, opacity = 1) {
    if (!(buf instanceof Buffer)) {
      throw new Error("Argument must be a buffer.");
    }
    if (isNaN(width) || isNaN(height)) {
      width = this.width;
      height = this.height;
    }
    x = Math.round(x);
    y = Math.round(y);
    if (isNaN(x) || isNaN(y)) {
      throw new Error("Invalid x and y coordinates.");
    }
    if (width * height * 4 != buf.length) {
      throw new Error("Buffer length does not match provided width and height * 4 (not an RGBA array?)");
    }
    this._enc.drawRGBAImage(buf, width, height, x, y, target_width, target_height, opacity);
  }

  /**
   * Finish the movie off
   */
  finalize() {
    if (this._frameOpen) {
      this.closeFrame();
    }
    this._enc.dispose();
    delete this._enc;
  }
};

// Export it
module.exports = FFEncode;