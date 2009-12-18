
#include "v8_func.h"

using namespace v8;

VALUE V8_C_Function;

VALUE V8_Wrap_Function(Handle<Function> f) {
  return V8_Ref_Create(V8_C_Function, f);
}