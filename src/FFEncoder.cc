#include "FFEncoder.h"
#include <v8.h>
#include <v8-platform.h>
#include <cstring>
#include <cstdlib>
#include <stddef.h>
#include <node.h>
#include <math.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"
#include <stdio.h>
#include <string.h>

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

  int pix_count = vec->width * vec->height;
  int frame_len = pix_count * 4;
  char* _blankSlate = new char[frame_len];
  for (int i = 0; i < pix_count; i++) {
    int offset = i * 4;
    _blankSlate[offset] = (char)(int)0;
    _blankSlate[offset + 1] = (char)(int)0;
    _blankSlate[offset + 2] = (char)(int)0;
    _blankSlate[offset + 3] = (char)(int)255;
  }
  vec->blank_slate = _blankSlate;

  // Return the wrapped javascript instance
  info.GetReturnValue().Set(info.Holder());
}

// Get a fully realized frame of video in the form of an RGBA Buffer
NAN_METHOD(FFEncoder::GetBufferFromFrame)
{
  // unwrap this Vector
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());
  int frame_width = info[1]->IntegerValue();
  int frame_height = info[2]->IntegerValue();
  utils::Rectangle targetRect = utils::size_image_to_fit(self->width, self->height, frame_width, frame_height);
  //std::cout << "Target Movie: " << self->width << ", " << self->height << "\n";
  //std::cout << "Frame: " << frame_width << ", " << frame_height << "\n";
  //std::cout << "Rect Final: " << targetRect.x << ", " << targetRect.y << ", " << targetRect.w << ", " << targetRect.h << " \n";

  v8::Local<v8::Object> bufferObj = info[0]->ToObject();
  char* bufferData = node::Buffer::Data(bufferObj);
  size_t bufferLength = node::Buffer::Length(bufferObj);
  
  int pix_count = self->width * self->height;
  int frame_len = pix_count * 4;

  // Set up a copy of the blank slate
  char * out_img = new char[frame_len];
  memcpy ( &out_img, &self->blank_slate, sizeof(self->blank_slate) );

  // Blit and size the image onto the frame
  utils::blt_image_onto_frame(out_img, self->width, self->height, bufferData, frame_width, frame_height, targetRect);

  //std::cout << "Final Byte Len: " << frame_len << ", " << destination_size << "\n";
  //strncpy(out_img, self->blank_slate, frame_len);

/*
  char * retval = new char[bufferLength];
  for(unsigned int i = 0; i < bufferLength; i++ ) {
      unsigned char c = (unsigned char)bufferData[i];
      int v = (int)c;
      v = v / 2;
      //std::cout << "FENCODE VALS: " << v << " \n";
      retval[i] = (char)v;
  }
*/
  info.GetReturnValue().Set(Nan::NewBuffer(out_img, frame_len).ToLocalChecked());
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