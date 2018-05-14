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

#ifndef FFENCODER_H
#define FFENCODER_H

#include <nan.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

// FFMPEG includes
#ifdef __cplusplus
extern "C" {
#endif
	#include "libavcodec/avcodec.h"
	#include "libavutil/opt.h"
	#include "libavutil/imgutils.h"
	#include "libswscale/swscale.h"
#ifdef __cplusplus
}
#endif

class FFEncoder : public Nan::ObjectWrap
{
public:
  int width;
  int height;
  int fps;
  int isopen;
  int pix_count;
  int frame_len;
  char* blank_slate;
  char* current_frame;
  std::string filename;

	FILE *m_f;
	AVCodecContext *m_c;
	AVFrame *m_frame;
	AVPacket *m_pkt;
	struct SwsContext *m_sws_context;
	int64_t m_num_frames;

  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(New);
  static NAN_METHOD(OpenFrame);
  static NAN_METHOD(CloseFrame);
  static NAN_METHOD(CenterRGBAImage);
  static NAN_METHOD(DrawRGBAImage);
  static NAN_METHOD(GetPNGOfFrame);
  static NAN_METHOD(GetBufferOfFrame);
  static NAN_METHOD(Dispose);

  static NAN_GETTER(HandleGetters);
  static NAN_SETTER(HandleSetters);

  static Nan::Persistent<v8::FunctionTemplate> constructor;
};

#endif