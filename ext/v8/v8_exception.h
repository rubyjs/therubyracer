#ifndef _RR_V8_EXCEPTION_
#define _RR_V8_EXCEPTION_

#include "v8.h"
#include "ruby.h"

void rr_init_v8_exception();
VALUE rr_reflect_v8_stacktrace(v8::Handle<v8::StackTrace> value);
VALUE rr_reflect_v8_stackframe(v8::Handle<v8::StackFrame> value);

#endif
