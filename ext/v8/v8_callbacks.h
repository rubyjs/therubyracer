#ifndef _RR_V8_CALLBACKS_
#define _RR_V8_CALLBACKS_
#include "v8.h"
void rr_init_v8_callbacks();
// VALUE rr_v82rb(v8::AccessorInfo& info);
// VALUE rr_v82rb(v8::Arguments& arguments);

v8::Handle<v8::Value> RubyInvocationCallback(const v8::Arguments& args);
#endif