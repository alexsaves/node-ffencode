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
  char* blank_slate;
  std::string filename;

  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(New);
  static NAN_METHOD(AddFrame);
  static NAN_METHOD(GetPNGFromFrame);
  static NAN_METHOD(GetBufferFromFrame);

  static NAN_GETTER(HandleGetters);
  static NAN_SETTER(HandleSetters);

  static Nan::Persistent<v8::FunctionTemplate> constructor;
};

#endif