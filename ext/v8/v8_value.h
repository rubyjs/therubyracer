#ifndef _RR_V8_VALUE_
#define _RR_V8_VALUE_ 

#include "rr.h"

void rr_init_value();
VALUE rr_wrap_v8_value(v8::Handle<v8::Value>& value);
VALUE rr_v8_value_class();
VALUE rr_v8_value_empty();
#endif
