#include "rr.h"
#include "v8_try_catch.h"
#include "v8_message.h"

using namespace v8;

namespace {
  VALUE TryCatchClass;

  TryCatch *unwrap(VALUE self) {
    TryCatch *tc = 0;
    Data_Get_Struct(self, class TryCatch, tc);
    if (RTEST(rb_iv_get(self, "dead"))) {
      rb_raise(rb_eScriptError, "out of scope access of %s", RSTRING_PTR(rb_inspect(self)));
      return false;
    } else {
      return tc;
    }
  }
  
  VALUE Try(int argc, VALUE *argv, VALUE self) {
    if (rb_block_given_p()) {
      HandleScope scope;
      TryCatch tc;
      VALUE try_catch = Data_Wrap_Struct(TryCatchClass, 0, 0, &tc);
      rb_iv_set(try_catch, "dead", Qfalse);
      VALUE result = rb_yield(try_catch);
      rb_iv_set(try_catch, "dead", Qtrue);
      tc.Reset();
      return result;
    } else {
      return Qnil;
    }
  }
  
  VALUE HasCaught(VALUE self) {
    TryCatch *tc = unwrap(self);
    return tc ? rr_v82rb(tc->HasCaught()) : Qnil;
  }
  
  VALUE _Exception(VALUE self) {
    TryCatch *tc = unwrap(self);
    return tc ? rr_v82rb(tc->Exception()) : Qnil;
  }
  
  VALUE _StackTrace(VALUE self) {
    TryCatch *tc = unwrap(self);
    return tc ? rr_v82rb(tc->StackTrace()) : Qnil;
  }    
  
  VALUE _Message(VALUE self) {
    TryCatch *tc = unwrap(self);
    return tc ? rr_v82rb(tc->Message()) : Qnil;
  }
  
  VALUE CanContinue(VALUE self) {
    TryCatch *tc = unwrap(self);
    return tc ? rr_v82rb(tc->CanContinue()) : Qnil;    
  }
}

void rr_init_v8_try_catch() {
  TryCatchClass = rr_define_class("TryCatch");
  rr_define_singleton_method(TryCatchClass, "try", Try, -1);
  rr_define_method(TryCatchClass, "HasCaught", HasCaught, 0);
  rr_define_method(TryCatchClass, "Exception", _Exception, 0);
  rr_define_method(TryCatchClass, "StackTrace", _StackTrace, 0);
  rr_define_method(TryCatchClass, "Message", _Message, 0);
  rr_define_method(TryCatchClass, "CanContinue", CanContinue, 0);
}
