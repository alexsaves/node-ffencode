#include <nan.h>

class FFEncoder : public Nan::ObjectWrap
{
public:
  int width;
  int height;
  int fps;

  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(New);

  static NAN_GETTER(HandleGetters);
  static NAN_SETTER(HandleSetters);

  static Nan::Persistent<v8::FunctionTemplate> constructor;
};