#include "rr.h"
#include "v8_try_catch.h"
#include "v8_msg.h"

using namespace v8;

namespace {
  VALUE TryCatchClass;

  struct try_catch {
    TryCatch *tc;
    bool alive;
    try_catch(TryCatch *ptr);
  };

  try_catch::try_catch(TryCatch *ptr) :tc(ptr) {
    this->alive = true;
  }
  
  struct kill_try_catch {
    try_catch *tc;
    kill_try_catch(try_catch *ptr);
    ~kill_try_catch();
  };
  kill_try_catch::kill_try_catch(try_catch *ptr) : tc(ptr) {}
  kill_try_catch::~kill_try_catch() {
    tc->alive = false;
  }
  
  
  void mark_try_catch(try_catch *tc) {
    //nothing to mark since the TryCatch object is stack allocated.
  }
  
  void free_try_catch(try_catch *ptr) {
    delete ptr;
  }
  
  TryCatch *unwrap(VALUE self) {
    try_catch *ref = 0;
    Data_Get_Struct(self, struct try_catch, ref);
    if (ref->alive) {
      return ref->tc;
    } else {
      rb_raise(rb_eScriptError, "out of scope access of %s", RSTRING_PTR(rb_inspect(self)));
      return false;
    }
  }
  
  VALUE Try(int argc, VALUE *argv, VALUE self) {
    if (rb_block_given_p()) {
      HandleScope scope;
      TryCatch tc;
      try_catch *wrapper = new try_catch(&tc);
      kill_try_catch kill_on_return(wrapper);
      VALUE rb_ref = Data_Wrap_Struct(TryCatchClass, mark_try_catch, free_try_catch, wrapper);
      VALUE result = rb_yield(rb_ref);
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
  rb_funcall(TryCatchClass, rb_intern("private_class_method"), 1, rb_str_new2("new"));
}
