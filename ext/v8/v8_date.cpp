#include "rr.h"
#include "v8_date.h"
#include "v8_value.h"
#include "v8_ref.h"

using namespace v8;

namespace {
  
  VALUE DateClass;
  
  VALUE New(VALUE self, VALUE time) {
    HandleScope scope;
    return rr_v8_ref_create(self, Date::New(NUM2DBL(rb_funcall(time, rb_intern("to_f"), 0))));
  }
}

void rr_init_v8_date() {
  DateClass = rr_define_class("Date", rr_cV8_C_Value);
  rr_define_singleton_method(DateClass, "New", New, 1);
}

VALUE rr_reflect_v8_date(Handle<Value> value) {
  Local<Date> date(Date::Cast(*value));
  return rr_v8_ref_create(DateClass, date);
}