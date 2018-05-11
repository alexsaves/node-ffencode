const ffencode = require('bindings')('ffencode');
const pjson = require('./package.json');

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

  }
};

// Export it
module.exports = FFEncode;