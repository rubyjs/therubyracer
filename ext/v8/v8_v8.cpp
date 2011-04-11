
#include "rr.h"

using namespace v8;

namespace {

  VALUE IsDead(VALUE self) {
    return rr_v82rb(V8::IsDead());
  }

  VALUE AdjustAmountOfExternalAllocatedMemory(VALUE self, VALUE bytes) {
    V8::AdjustAmountOfExternalAllocatedMemory(NUM2INT(bytes));
    return Qnil;
  }

   VALUE IdleNotification(VALUE self) {
     return rr_v82rb(V8::IdleNotification());
   }

  VALUE SetFlagsFromString(VALUE self, VALUE string) {
    V8::SetFlagsFromString(RSTRING_PTR(string), RSTRING_LEN(string));
    return Qnil;
  }

}


void rr_init_v8_v8() {
  VALUE V8Module = rr_define_module("V8");
  rr_define_singleton_method(V8Module, "IsDead", IsDead, 0);
  rr_define_singleton_method(V8Module, "AdjustAmountOfExternalAllocatedMemory", AdjustAmountOfExternalAllocatedMemory, 1);
  rr_define_singleton_method(V8Module, "IdleNotification", IdleNotification, 0);
  rr_define_singleton_method(V8Module, "SetFlagsFromString", SetFlagsFromString, 1);
}
