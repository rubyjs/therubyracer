
#include "v8_cxt.h"
#include "ruby.h"

using namespace v8;

VALUE v8_Context_New(int argc, VALUE *argv, VALUE self) {
  HandleScope handles;
  VALUE scope;
  rb_scan_args(argc,argv, "01", &scope);
  if (NIL_P(scope)) {
    return V8_Ref_Create(self, Context::New());
  } else {
    V8_Ref_Get(ObjectTemplate, t, scope);
    return V8_Ref_Create(self, Context::New(0, t));
  }
}

VALUE v8_cxt_Global(VALUE self) {
  V8_Ref_Get(Context, cxt, self);
  cxt->Global();
  return Qnil;  
}

VALUE v8_cxt_open(VALUE self) {
  HandleScope handles;
  V8_Ref_Get(Context, cxt, self);
  Context::Scope enter(cxt);
  if (rb_block_given_p()) {
    return rb_yield(self);
  } else {
    return Qnil;
  }
}


