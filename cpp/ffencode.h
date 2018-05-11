#ifndef __FFENCODE_DEFINED__
#define __FFENCODE_DEFINED__

#include <nan.h>

class FFEncode {
public:
    FFEncode(int, int, int);

    ~FFEncode();

    int width, height, fps;
};

#endif