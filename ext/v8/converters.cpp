#include "converters.h"

VALUE V82RB(v8::Handle<v8::Value>& value) {
  convert_v8_to_rb_t convert;
  return convert(value);
}

v8::Local<v8::Value> RB2V8(VALUE value) {
  convert_rb_to_v8_t convert;
  return convert(value);
}

std::string RB2String(VALUE value) {
  convert_rb_to_string_t convert;
  return convert(value);
}

std::string V82String(v8::Handle<v8::Value>& value) {
  convert_v8_to_string_t convert;
  return convert(value);
}