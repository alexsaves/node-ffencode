#include "FFEncoder.h"
#include <v8.h>
#include <v8-platform.h>
#include <cstring>
#include <cstdlib>

Nan::Persistent<v8::FunctionTemplate> FFEncoder::constructor;

// Initializer ********
NAN_MODULE_INIT(FFEncoder::Init)
{
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(FFEncoder::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("FFEncoder").ToLocalChecked());

  // link our getters and setter to the object property
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("width").ToLocalChecked(), FFEncoder::HandleGetters, FFEncoder::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("height").ToLocalChecked(), FFEncoder::HandleGetters, FFEncoder::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("fps").ToLocalChecked(), FFEncoder::HandleGetters, FFEncoder::HandleSetters);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("filename").ToLocalChecked(), FFEncoder::HandleGetters, FFEncoder::HandleSetters);

  target->Set(Nan::New("FFEncoder").ToLocalChecked(), ctor->GetFunction());
}

// Constructor *************
NAN_METHOD(FFEncoder::New)
{
  v8::Isolate* isolate = info.GetIsolate();

  // throw an error if constructor is called without new keyword
  if (!info.IsConstructCall())
  {
    return Nan::ThrowError(Nan::New("FFEncoder::New - called without new keyword").ToLocalChecked());
  }

  // expect exactly 4 arguments
  if (info.Length() != 4)
  {
    return Nan::ThrowError(Nan::New("FFEncoder::New - expected arguments width, height, fps, filename").ToLocalChecked());
  }

  // expect arguments to be numbers
  if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber() || !info[3]->IsString())
  {
    return Nan::ThrowError(Nan::New("FFEncoder::New - width, height, fps must be numbers and filename must be a string").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance
  FFEncoder *vec = new FFEncoder();
  vec->Wrap(info.Holder());

  // initialize it's values
  vec->width = info[0]->NumberValue();
  vec->height = info[1]->NumberValue();
  vec->fps = info[2]->NumberValue();
  vec->filename = *v8::String::Utf8Value(isolate, info[3]);
  //std::string attribute = *v8::String::Utf8Value(isolate, info[3]);
  //std::string name = *v8::String::Utf8Value(info[3]->ToString());
  //vec->filename = info[3]->ToString();
  //v8::String::String param1(info[3]->ToString());
  //vec->filename = std::string(*param1);
  //v8::String::Utf8String val(info[3]->ToString());

  // return the wrapped javascript instance
  info.GetReturnValue().Set(info.Holder());
}

NAN_GETTER(FFEncoder::HandleGetters)
{
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());

  std::string propertyName = std::string(*Nan::Utf8String(property));
  if (propertyName == "width")
  {
    info.GetReturnValue().Set(self->width);
  }
  else if (propertyName == "height")
  {
    info.GetReturnValue().Set(self->height);
  }
  else if (propertyName == "fps")
  {
    info.GetReturnValue().Set(self->fps);
  }
  else if (propertyName == "filename")
  {
    v8::Local<v8::String> hTextJS = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), self->filename.c_str());
    info.GetReturnValue().Set(hTextJS);
  }
  else
  {
    info.GetReturnValue().Set(Nan::Undefined());
  }
}

NAN_SETTER(FFEncoder::HandleSetters)
{
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());

  if (!value->IsNumber())
  {
    return Nan::ThrowError(Nan::New("expected value to be a number").ToLocalChecked());
  }

  std::string propertyName = std::string(*Nan::Utf8String(property));
  if (propertyName == "width")
  {
    self->width = value->NumberValue();
  }
  else if (propertyName == "height")
  {
    self->height = value->NumberValue();
  }
  else if (propertyName == "fps")
  {
    self->fps = value->NumberValue();
  }
}