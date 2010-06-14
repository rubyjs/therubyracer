#ifndef _RR_V8_VALUE_
#define _RR_V8_VALUE_ 

#include "rr.h"

extern VALUE rr_cV8_C_Value;
extern VALUE rr_cV8_C_Empty;
void rr_init_value();
VALUE rr_wrap_v8_value(v8::Handle<v8::Value>& value);
#endif
