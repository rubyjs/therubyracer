#ifndef __converters_h__
#define __converters_h__

#include "convert_ruby.h"
#include "convert_string.h"
#include "convert_v8.h"
#include <cstring>

extern VALUE V8_To;

typedef RubyValueSource<V8LocalDest, v8::Local<v8::Value> > convert_rb_to_v8_t; 
typedef V8HandleSource<RubyValueDest, VALUE> convert_v8_to_rb_t;

typedef RubyValueSource<StringDest, std::string> convert_rb_to_string_t;
typedef V8HandleSource<StringDest, std::string> convert_v8_to_string_t;

VALUE V82RB(v8::Handle<v8::Value>& value);
v8::Local<v8::Value> RB2V8(VALUE value);

std::string RB2String(VALUE value);
std::string V82String(v8::Handle<v8::Value>& value);

v8::Local<v8::ObjectTemplate> RB_VALUE_2_V8_ObjectTemplate(VALUE value);

#endif