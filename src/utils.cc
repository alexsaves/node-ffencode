#include "utils.h"
#include <stddef.h>
#include <node.h>
#include <math.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>

namespace utils
{
  // Size a rectangle to fit within another one
  Rectangle size_image_to_fit(int width, int height, int image_width, int image_height) {
      Rectangle out_rect;

      float src_aspect = (float)width / (float)height;
      float frame_aspect = (float)image_width / (float)image_height;
      //std::cout << "src_aspect: " << src_aspect << " - frame_aspect: " << frame_aspect << "\n";
      if (frame_aspect > src_aspect) {
        out_rect.w = width;
        out_rect.h = (int)(((float)width / (float)image_width) * (float)image_height);
        //out_rect.h = (int)fmax((((float)width / (float)image_width)) * (float)image_height, (float)height);
        out_rect.x = 0;
        out_rect.y = (int)(((float)height - (float)out_rect.h) / 2.0);
      } else {
        out_rect.h = height;
        out_rect.w = (int)(((float)height / (float)image_height) * (float)image_width);
        out_rect.y = 0;
        out_rect.x = (int)(((float)width - (float)out_rect.w) / 2.0);
      }
      //float final_aspect = (float)out_rect.w / (float)out_rect.h;
      //std::cout << "final_aspect: " << final_aspect << "\n";
      return out_rect;
  }

  // Draw an image onto another image
  void blt_image_onto_frame(char* base_frame, int frame_width, int frame_height, char* image, int image_width, int image_height, Rectangle where) {
    std::cout << "Rect Final: " << where.x << ", " << where.y << ", " << where.w << ", " << where.h << " | frame: " << image_width << ", " << image_height << "\n";
    for (int x = where.x; x < where.x + where.w; x++) {
      for (int y = where.y; y < where.y + where.h; y++) {
        int r = 200;
        int g = 0;
        int b = 0;
        int a = 255;

        int offset = ((y * frame_width) + x) * 4;
        base_frame[offset++] = (char)r;
        base_frame[offset++] = (char)g;
        base_frame[offset++] = (char)b;
        base_frame[offset] = (char)a;
      }
    }
  }
}