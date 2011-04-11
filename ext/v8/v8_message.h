#ifndef _RUBY_V8_MESSAGE_
#define _RUBY_V8_MESSAGE_ 

#include "v8.h"
#include "rr.h"

void rr_init_message();
VALUE rr_reflect_v8_message(v8::Handle<v8::Message> value);

#endif
