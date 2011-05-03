#include "rr.h"
#include "v8_date.h"
#include "v8_value.h"
#include "v8_handle.h"
#include "v8_object.h"

using namespace v8;

namespace {

  VALUE DateClass;

  VALUE New(VALUE self, VALUE time) {
    HandleScope scope;
    return rr_v8_handle_new(self, Date::New(NUM2DBL(time)));
  }

  // Override Value::NumberValue in order to ensure that we call the more specific and optimized
  // Number Value in v8::Date
  VALUE NumberValue(VALUE self) {
    Persistent<Date> date = rr_v8_handle<Date>(self);
    return rr_v82rb(date->NumberValue());
  }
}

void rr_init_v8_date() {
  DateClass = rr_define_class("Date", rr_v8_value_class());
  rr_define_singleton_method(DateClass, "New", New, 1);
  rr_define_method(DateClass, "NumberValue", NumberValue, 0);
}

VALUE rr_reflect_v8_date(Handle<Value> value) {
  return rr_reflect_v8_object_as(Handle<Object>::Cast(value), DateClass);
}