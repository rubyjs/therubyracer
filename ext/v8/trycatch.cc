#include "rr.h"

namespace rr {
  VALUE TryCatch::Class;

  void TryCatch::Init() {
    ClassBuilder("TryCatch").
      defineMethod("HasCaught", &HasCaught).
      defineMethod("CanContinue", &CanContinue).
      defineMethod("ReThrow", &ReThrow).
      defineMethod("Exception", &Exception).
      defineMethod("StackTrace", &StackTrace).
      defineMethod("Message", &Message).
      defineMethod("Reset", &Reset).
      defineMethod("SetVerbose", &SetVerbose).
      defineMethod("SetCaptureMessage", &SetCaptureMessage).
      store(&Class);
    VALUE v8 = rb_define_module("V8");
    VALUE c = rb_define_module_under(v8, "C");
    rb_define_singleton_method(c, "TryCatch", (VALUE (*)(...))&doTryCatch, -1);
  }

  TryCatch::TryCatch() : impl(new v8::TryCatch()), allocated(true) {}
  TryCatch::TryCatch(VALUE value) : allocated(false) {
    Data_Get_Struct(value, class v8::TryCatch, impl);
  }
  TryCatch::~TryCatch() {
    if (this->allocated) {
      delete this->impl;
    }
  }
  TryCatch::operator VALUE() {
    return Data_Wrap_Struct(Class, 0, 0, impl);
  }

  VALUE TryCatch::HasCaught(VALUE self) {
    return Bool(TryCatch(self)->HasCaught());
  }
  VALUE TryCatch::CanContinue(VALUE self) {
    return Bool(TryCatch(self)->CanContinue());
  }
  VALUE TryCatch::ReThrow(VALUE self) {
    return Value(TryCatch(self)->ReThrow());
  }
  VALUE TryCatch::Exception(VALUE self) {
    return Value(TryCatch(self)->Exception());
  }
  VALUE TryCatch::StackTrace(VALUE self) {
    return Value(TryCatch(self)->StackTrace());
  }
  VALUE TryCatch::Message(VALUE self) {
    return rr::Message(TryCatch(self)->Message());
  }
  VALUE TryCatch::Reset(VALUE self) {
    Void(TryCatch(self)->Reset());
  }
  VALUE TryCatch::SetVerbose(VALUE self, VALUE value) {
    Void(TryCatch(self)->SetVerbose(Bool(value)));
  }
  VALUE TryCatch::SetCaptureMessage(VALUE self, VALUE value) {
    Void(TryCatch(self)->SetCaptureMessage(Bool(value)));
  }

  VALUE TryCatch::doTryCatch(int argc, VALUE argv[], VALUE self) {
    if (!rb_block_given_p()) {
      return Qnil;
    }
    int state = 0;
    VALUE code;
    rb_scan_args(argc,argv,"00&", &code);
    VALUE result = setupAndCall(&state, code);
    if (state != 0) {
      rb_jump_tag(state);
    }
    return result;
  }

  VALUE TryCatch::setupAndCall(int* state, VALUE code) {
    return rb_protect(&doCall, code, state);
  }

  VALUE TryCatch::doCall(VALUE code) {
    TryCatch trycatch;
    return rb_funcall(code, rb_intern("call"), 1, (VALUE)trycatch);
  }
}