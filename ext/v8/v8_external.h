#ifndef _RR_V8_EXTERNAL_
#define _RR_V8_EXTERNAL_

#include "rr.h"

void rr_init_v8_external();
VALUE rr_reflect_v8_external(v8::Handle<v8::Value> value);
#endif
