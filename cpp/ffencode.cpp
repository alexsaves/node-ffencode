#include "ffencode.h"
#include <stddef.h>
#include <node.h>
#include <math.h>
#include <iostream>

/**
 * Constructor for a new sparse matrix
 */
FFEncode::FFEncode(int W, int H, int FPS) {
  width = W;
  height = H;
  fps = FPS;
}
