#ifndef _RUBY_V8_MESSAGE_
#define _RUBY_V8_MESSAGE_ 

#include "v8.h"
#include "ruby.h"

extern VALUE V8_C_Message;
VALUE V8_Wrap_Message(v8::Handle<v8::Message> msg);
VALUE v8_Message_Get(VALUE self);
#endif