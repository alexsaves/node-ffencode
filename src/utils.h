#ifndef FFUTILS_H
#define FFUTILS_H

namespace utils
{
  // Defines a rectangle
  struct Rectangle {
      int x;
      int y;
      int w;
      int h;
  };

  // Fit one rectangle into another and position it nicely
  Rectangle size_image_to_fit(int width, int height, int image_width, int image_height);

  // Draw an image onto another image
  void blt_image_onto_frame(char* base_frame, int frame_width, int frame_height, char* image, int image_width, int image_height, Rectangle where);
}

#endif