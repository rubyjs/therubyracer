
#include "v8_cxt.h"
#include "ruby.h"

using namespace v8;

Local<Context> VALUE_TO_CONTEXT(VALUE value) {
  v8_ref* ref = 0;
  Data_Get_Struct(value, struct v8_ref, ref);
  return (Context *)(*ref->handle);
}

VALUE v8_cxt_allocate(VALUE clazz) {
  v8_ref* ref = new v8_ref(Context::New());
  return Data_Wrap_Struct(clazz, v8_ref_mark , v8_ref_free, ref);
}

VALUE v8_cxt_Global(VALUE self) {
  Local<Context> cxt = VALUE_TO_CONTEXT(self);
  cxt->Global();
  return Qnil;  
}

VALUE v8_cxt_open(VALUE self) {
  HandleScope handles;
  Local<Context> cxt = VALUE_TO_CONTEXT(self);
  Context::Scope enter(cxt);
  if (rb_block_given_p()) {
    return rb_yield(self);
  } else {
    return Qnil;
  }
}


