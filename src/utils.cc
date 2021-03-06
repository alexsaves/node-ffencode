/*
* FFEncode
* MIT License
* 
* Copyright (c) 2018 Alexei White
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "utils.h"
#include <stddef.h>
#include <node.h>
#include <math.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <cmath>

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
  void blt_image_onto_frame(char* base_frame, int frame_width, int frame_height, char* image, int image_width, int image_height, Rectangle where, float opacity) {
    //std::cout << "Rect Final: " << where.x << ", " << where.y << ", " << where.w << ", " << where.h << " | frame: " << image_width << ", " << image_height << "\n";
    
    float src_y = 0;
    float src_x = 0;

    // Top left
    int src_y_1 = 0;
    int src_x_1 = 0;

    // Top right
    int src_y_2 = 0;
    int src_x_2 = 0;

    // Bottom left
    int src_y_3 = 0;
    int src_x_3 = 0;

    // Bottom right
    int src_y_4 = 0;
    int src_x_4 = 0;

    float imgw = (float)image_width;
    float imgh = (float)image_height; 
    float wherew = (float)where.w;
    float whereh = (float)where.h; 
    float x_prog;
    float y_prog; 
    int src_offset; 
    float r1;
    float g1;
    float b1;
    float a1; 
    float r2;
    float g2;
    float b2;
    float a2;
    float r3;
    float g3;
    float b3;
    float a3;
    float r4;
    float g4;
    float b4;
    float a4;
    float rt;
    float gt;
    float bt;
    float at;
    float rb;
    float gb;
    float bb;
    float ab;
    int rf;
    int gf;
    int bf;
    int af;
    int dest_offset;
    float src_r;
    float src_g;
    float src_b;
    float src_a;
    float opac_prog;
    for (int y = where.y; y < where.y + where.h; y++) {
      if (y > 0 && y < frame_height) {    
        for (int x = where.x; x < where.x + where.w; x++) {
          if (x > 0 && x < frame_width) {  
            // Find the precise position to sample
            src_x = ((float)(x - where.x) / wherew) * imgw;
            src_y = ((float)(y - where.y) / whereh) * imgh;

            // Figure out the four corners
            src_x_1 = (int)fmin(imgw - 1, fmax(0, floor(src_x)));
            src_y_1 = (int)fmin(imgh - 1, fmax(0, floor(src_y)));
            src_x_2 = (int)fmin(imgw - 1, fmax(0, ceil(src_x)));
            src_y_2 = src_y_1;
            src_x_3 = src_x_1;
            src_y_3 = (int)fmin(imgh - 1, fmax(0, ceil(src_y)));
            src_x_4 = src_x_2;
            src_y_4 = src_y_3;

            x_prog = 1.0 - (src_x - (float)floor(src_x));
            y_prog = 1.0 - (src_y - (float)floor(src_y));

            src_offset = ((src_y_1 * image_width) + src_x_1) * 4;
            
            r1 = (float)(int)(unsigned char)image[src_offset++];
            g1 = (float)(int)(unsigned char)image[src_offset++];
            b1 = (float)(int)(unsigned char)image[src_offset++];
            a1 = (float)(int)(unsigned char)image[src_offset];

            src_offset = ((src_y_2 * image_width) + src_x_2) * 4;

            r2 = (float)(int)(unsigned char)image[src_offset++];
            g2 = (float)(int)(unsigned char)image[src_offset++];
            b2 = (float)(int)(unsigned char)image[src_offset++];
            a2 = (float)(int)(unsigned char)image[src_offset];

            src_offset = ((src_y_3 * image_width) + src_x_3) * 4;

            r3 = (float)(int)(unsigned char)image[src_offset++];
            g3 = (float)(int)(unsigned char)image[src_offset++];
            b3 = (float)(int)(unsigned char)image[src_offset++];
            a3 = (float)(int)(unsigned char)image[src_offset];

            src_offset = ((src_y_4 * image_width) + src_x_4) * 4;

            r4 = (float)(int)(unsigned char)image[src_offset++];
            g4 = (float)(int)(unsigned char)image[src_offset++];
            b4 = (float)(int)(unsigned char)image[src_offset++];
            a4 = (float)(int)(unsigned char)image[src_offset];
            
            // Sum the top row
            rt = ((r1 * x_prog) + (r2 * (1-x_prog)));
            gt = ((g1 * x_prog) + (g2 * (1-x_prog)));
            bt = ((b1 * x_prog) + (b2 * (1-x_prog)));
            at = ((a1 * x_prog) + (a2 * (1-x_prog)));

            // Sum the bottom row
            rb = ((r3 * x_prog) + (r4 * (1-x_prog)));
            gb = ((g3 * x_prog) + (g4 * (1-x_prog)));
            bb = ((b3 * x_prog) + (b4 * (1-x_prog)));
            ab = ((a3 * x_prog) + (a4 * (1-x_prog)));

            // Sum the two rows
            rf = (int)((rt * y_prog) + (rb * (1-y_prog)));
            gf = (int)((gt * y_prog) + (gb * (1-y_prog)));
            bf = (int)((bt * y_prog) + (bb * (1-y_prog)));
            af = (int)((at * y_prog) + (ab * (1-y_prog)));

            dest_offset = ((y * frame_width) + x) * 4;

            if (af == 255 && opacity == 1) {
              base_frame[dest_offset++] = (char)rf;
              base_frame[dest_offset++] = (char)gf;
              base_frame[dest_offset++] = (char)bf;
              base_frame[dest_offset] = (char)af;
            } else {
              src_r = (float)(int)(unsigned char)base_frame[dest_offset];
              src_g = (float)(int)(unsigned char)base_frame[dest_offset + 1];
              src_b = (float)(int)(unsigned char)base_frame[dest_offset + 2];
              src_a = (float)(int)(unsigned char)base_frame[dest_offset + 3];
              opac_prog = ((float)af / 255) * opacity;
              rf =  (int)((((float)rf * opac_prog) + (src_r * (1-opac_prog))));
              gf =  (int)((((float)gf * opac_prog) + (src_g * (1-opac_prog))));
              bf =  (int)((((float)bf * opac_prog) + (src_b * (1-opac_prog))));
              af =  (int)((((float)af * opac_prog) + (src_a * (1-opac_prog))));
              base_frame[dest_offset++] = (char)rf;
              base_frame[dest_offset++] = (char)gf;
              base_frame[dest_offset++] = (char)bf;
              base_frame[dest_offset] = (char)af;
            }
          }
        }
      }
    }
  }
}