#include "rr.h"
#include "v8_date.h"
#include "v8_value.h"
#include "v8_ref.h"

using namespace v8;

namespace {

  VALUE DateClass;

  VALUE New(VALUE self, VALUE time) {
    HandleScope scope;
    return rr_v8_ref_create(self, Date::New(NUM2DBL(time)));
  }

  // Override Value::NumberValue in order to ensure that we call the more specific and optimized
  // Number Value in v8::Date
  VALUE NumberValue(VALUE self) {
    HandleScope scope;
    Local<Date> date = V8_Ref_Get<Date>(self);
    return rr_v82rb(date->NumberValue());
  }
}

void rr_init_v8_date() {
  DateClass = rr_define_class("Date", rr_cV8_C_Value);
  rr_define_singleton_method(DateClass, "New", New, 1);
  rr_define_method(DateClass, "NumberValue", NumberValue, 0);
}

VALUE rr_reflect_v8_date(Handle<Value> value) {
  Local<Date> date(Date::Cast(*value));
  return rr_v8_ref_create(DateClass, date);
}