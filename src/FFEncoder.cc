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

#include "FFEncoder.h"
#include "lodepng.h"
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

  Nan::SetPrototypeMethod(ctor, "openFrame", OpenFrame);
  Nan::SetPrototypeMethod(ctor, "closeFrame", CloseFrame);
  Nan::SetPrototypeMethod(ctor, "centerRGBAImage", CenterRGBAImage);
  Nan::SetPrototypeMethod(ctor, "getPNGOfFrame", GetPNGOfFrame);
  Nan::SetPrototypeMethod(ctor, "drawRGBAImage", DrawRGBAImage);
  Nan::SetPrototypeMethod(ctor, "getBufferOfFrame", GetBufferOfFrame);
  Nan::SetPrototypeMethod(ctor, "dispose", Dispose);

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
  vec->isopen = 0;
  vec->pix_count = vec->width * vec->height;
  vec->frame_len = vec->pix_count * 4;
  vec->filename = *v8::String::Utf8Value(isolate, info[3]);
  vec->m_num_frames = 0;

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

// Free up memory
NAN_METHOD(FFEncoder::Dispose)
{
  //FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());
  //delete [] self->blank_slate;
}

// Start a frame 
NAN_METHOD(FFEncoder::OpenFrame)
{
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());
  if (self->isopen != 0)
  {
    return Nan::ThrowError(Nan::New("FFEncoder::OpenFrame - frame was already open").ToLocalChecked());
  }
  self->isopen = 1;

  // Set up a copy of the blank slate
  self->current_frame = new char[self->frame_len];
  memcpy ( &self->current_frame, &self->blank_slate, sizeof(self->blank_slate) );
}

// Close a frame 
NAN_METHOD(FFEncoder::CloseFrame)
{
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());
  if (self->isopen != 1)
  {
    return Nan::ThrowError(Nan::New("FFEncoder::OpenFrame - frame was already closed").ToLocalChecked());
  }
  self->isopen = 0;
  delete [] self->current_frame;
}

// Get a fully realized frame of video in the form of an RGBA Buffer
NAN_METHOD(FFEncoder::CenterRGBAImage)
{
  // unwrap this Vector
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());
  int frame_width = info[1]->IntegerValue();
  int frame_height = info[2]->IntegerValue();
  utils::Rectangle targetRect = utils::size_image_to_fit(self->width, self->height, frame_width, frame_height);
  
  v8::Local<v8::Object> bufferObj = info[0]->ToObject();
  char* bufferData = node::Buffer::Data(bufferObj);
  
  // Blit and size the image onto the frame
  utils::blt_image_onto_frame(self->current_frame, self->width, self->height, bufferData, frame_width, frame_height, targetRect, 1);
}

// Draw an image at a specific place
NAN_METHOD(FFEncoder::DrawRGBAImage) 
{
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());
  int frame_width = info[1]->IntegerValue();
  int frame_height = info[2]->IntegerValue();
  utils::Rectangle targetRect;
  targetRect.x = info[3]->IntegerValue();
  targetRect.y = info[4]->IntegerValue();
  targetRect.w = info[5]->IntegerValue();
  targetRect.h = info[6]->IntegerValue();
  float opacity = (float)info[7]->NumberValue();
  v8::Local<v8::Object> bufferObj = info[0]->ToObject();
  char* bufferData = node::Buffer::Data(bufferObj);
  // Blit and size the image onto the frame
  utils::blt_image_onto_frame(self->current_frame, self->width, self->height, bufferData, frame_width, frame_height, targetRect, opacity);
}

// Get a PNG of the current frame
NAN_METHOD(FFEncoder::GetPNGOfFrame) 
{
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());
  unsigned char* convert_var = new unsigned char[self->frame_len];
  memcpy ( &convert_var, &self->current_frame, sizeof(self->current_frame) );

  // ENCODE AS PNG
  std::vector<unsigned char> outVect;
  lodepng::encode(outVect, convert_var, (unsigned)self->width, (unsigned)self->height);
  char* myArr = new char[outVect.size()];
  std::copy(outVect.begin(), outVect.end(), myArr);

  info.GetReturnValue().Set(Nan::NewBuffer(myArr, outVect.size()).ToLocalChecked());
}

// Get an uncompressed RGBA buffer of the current frame
NAN_METHOD(FFEncoder::GetBufferOfFrame) 
{
  FFEncoder *self = Nan::ObjectWrap::Unwrap<FFEncoder>(info.This());
  info.GetReturnValue().Set(Nan::NewBuffer(self->current_frame, self->frame_len).ToLocalChecked());
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