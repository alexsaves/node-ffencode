#include <nan.h>
#include "Vector.h"
#include "FFEncoder.h"

NAN_MODULE_INIT(InitModule)
{
  Vector::Init(target);
  FFEncoder::Init(target);
}

NODE_MODULE(myModule, InitModule);