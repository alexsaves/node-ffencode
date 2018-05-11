/**
 * Build a fake image buffer
 * @param {Number} width 
 * @param {Number} height 
 */
const MakeFakeImageRGBABuffer = function(width, height) {
  const pixlen = width * height;
  const arr = new Uint8Array(pixlen * 4);
  for (let i = 0; i < pixlen; i++) {
    arr[(i * 4)] = Math.round((Math.random() * 100) + 155);
    arr[(i * 4) + 1] = Math.round((Math.random() * 100) + 155);
    arr[(i * 4) + 2] = Math.round((Math.random() * 100) + 155);
    arr[(i * 4) + 3] = 255;
  }
  return ArrayBuffer.from(arr);
};

// Export it
module.exports = {MakeFakeImageBuffer};