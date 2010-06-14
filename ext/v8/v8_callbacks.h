#ifndef _RR_V8_CALLBACKS_
#define _RR_V8_CALLBACKS_
#include "rr.h"
void rr_init_v8_callbacks();
VALUE rr_v82rb(const v8::AccessorInfo& info);
VALUE rr_v82rb(const v8::Arguments& arguments);

#endif
