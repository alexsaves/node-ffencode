#include <nan.h>
#include "FFEncoder.h"

NAN_MODULE_INIT(InitModule)
{
  FFEncoder::Init(target);
}

NODE_MODULE(myModule, InitModule);