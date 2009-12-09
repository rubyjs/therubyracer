
#include "v8_cxt.h"
#include "ruby.h"

using namespace v8;

VALUE v8_cxt_allocate(VALUE clazz) {
  return V8_Ref_Create(clazz, Context::New());
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


