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

  Nan::SetPrototypeMethod(ctor, "addFrame", AddFrame);
  Nan::SetPrototypeMethod(ctor, "getPNGFromFrame", GetPNGFromFrame);
  Nan::SetPrototypeMethod(ctor, "getBufferFromFrame", GetBufferFromFrame);

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
  vec->width = info[0]->IntegerValue();
  vec->height = info[1]->IntegerValue();
  vec->fps = info[2]->IntegerValue();
  vec->filename = *v8::String::Utf8Value(isolate, info[3]);

  // Return the wrapped javascript instance
  info.GetReturnValue().Set(info.Holder());
}

// Get a fully realized frame of video in the form of an RGBA Buffer
NAN_METHOD(FFEncoder::GetBufferFromFrame)
{
  // unwrap this Vector
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());
/*
  if (!Nan::New(FFEncoder::constructor)->HasInstance(info[0]))
  {
    return Nan::ThrowError(Nan::New("Vector::Add - expected argument to be instance of Vector").ToLocalChecked());
  }
  // unwrap the Vector passed as argument
  Vector *otherVec = Nan::ObjectWrap::Unwrap<Vector>(info[0]->ToObject());

  // specify argument counts and constructor arguments
  const int argc = 3;
  v8::Local<v8::Value> argv[argc] = {
      Nan::New(self->x + otherVec->x),
      Nan::New(self->y + otherVec->y),
      Nan::New(self->z + otherVec->z)};

  // get a local handle to our constructor function
  v8::Local<v8::Function> constructorFunc = Nan::New(Vector::constructor)->GetFunction();
  // create a new JS instance from arguments
  v8::Local<v8::Object> jsSumVec = Nan::NewInstance(constructorFunc, argc, argv).ToLocalChecked();

  info.GetReturnValue().Set(jsSumVec);*/
}

// Get a fully realized frame of video in the form of a PNG
NAN_METHOD(FFEncoder::GetPNGFromFrame)
{
  // unwrap this Vector
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());
/*
  if (!Nan::New(FFEncoder::constructor)->HasInstance(info[0]))
  {
    return Nan::ThrowError(Nan::New("Vector::Add - expected argument to be instance of Vector").ToLocalChecked());
  }
  // unwrap the Vector passed as argument
  Vector *otherVec = Nan::ObjectWrap::Unwrap<Vector>(info[0]->ToObject());

  // specify argument counts and constructor arguments
  const int argc = 3;
  v8::Local<v8::Value> argv[argc] = {
      Nan::New(self->x + otherVec->x),
      Nan::New(self->y + otherVec->y),
      Nan::New(self->z + otherVec->z)};

  // get a local handle to our constructor function
  v8::Local<v8::Function> constructorFunc = Nan::New(Vector::constructor)->GetFunction();
  // create a new JS instance from arguments
  v8::Local<v8::Object> jsSumVec = Nan::NewInstance(constructorFunc, argc, argv).ToLocalChecked();

  info.GetReturnValue().Set(jsSumVec);*/
}

// Add a frame to the video stream
NAN_METHOD(FFEncoder::AddFrame)
{
  // unwrap this Vector
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());
/*
  if (!Nan::New(FFEncoder::constructor)->HasInstance(info[0]))
  {
    return Nan::ThrowError(Nan::New("Vector::Add - expected argument to be instance of Vector").ToLocalChecked());
  }
  // unwrap the Vector passed as argument
  Vector *otherVec = Nan::ObjectWrap::Unwrap<Vector>(info[0]->ToObject());

  // specify argument counts and constructor arguments
  const int argc = 3;
  v8::Local<v8::Value> argv[argc] = {
      Nan::New(self->x + otherVec->x),
      Nan::New(self->y + otherVec->y),
      Nan::New(self->z + otherVec->z)};

  // get a local handle to our constructor function
  v8::Local<v8::Function> constructorFunc = Nan::New(Vector::constructor)->GetFunction();
  // create a new JS instance from arguments
  v8::Local<v8::Object> jsSumVec = Nan::NewInstance(constructorFunc, argc, argv).ToLocalChecked();

  info.GetReturnValue().Set(jsSumVec);*/
}

// Property getters *****************
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

// Property setters ****************************
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
    self->width = value->IntegerValue();
  }
  else if (propertyName == "height")
  {
    self->height = value->IntegerValue();
  }
  else if (propertyName == "fps")
  {
    self->fps = value->IntegerValue();
  }
}