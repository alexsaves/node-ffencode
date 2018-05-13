const pjson = require('./package.json');
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
    this.width = Math.round(width);
    this.height = Math.round(height);
    this.fps = Math.max(1, Math.min(120, Math.round(fps)));
    this.filename = filename;
    this._enc = new FFEncoder(this.width, this.height, this.fps);
    console.log(this._enc);
  }

  /**
   * Add a frame for the video
   * @param {Buffer} buf 
   */
  addRGBABufferFrame(buf) {

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