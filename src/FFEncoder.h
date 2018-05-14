#ifndef FFENCODER_H
#define FFENCODER_H

#include <nan.h>
#include <string>

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

  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(New);
  static NAN_METHOD(OpenFrame);
  static NAN_METHOD(CloseFrame);
  static NAN_METHOD(CenterRGBAImage);
  static NAN_METHOD(DrawRGBAImage);
  static NAN_METHOD(GetPNGOfFrame);

  static NAN_GETTER(HandleGetters);
  static NAN_SETTER(HandleSetters);

  static Nan::Persistent<v8::FunctionTemplate> constructor;
};

#endif