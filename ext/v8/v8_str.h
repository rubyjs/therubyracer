#ifndef _RUBY_V8_STR_
#define _RUBY_V8_STR_

#include "rr.h"

void rr_init_str();

VALUE rr_reflect_v8_string(v8::Handle<v8::Value> value);
#endif
