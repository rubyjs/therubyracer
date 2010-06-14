#ifndef _RR_V8_ARRAY_
#define _RR_V8_ARRAY_ 

#include "rr.h"

void rr_init_v8_array();
VALUE rr_reflect_v8_array(v8::Handle<v8::Value> value);
#endif
