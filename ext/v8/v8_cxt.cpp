#include "v8_cxt.h"
#include "v8_msg.h"
#include "converters.h"

using namespace v8;

VALUE v8_Context_New(int argc, VALUE *argv, VALUE self) {
  HandleScope handles;
  VALUE scope;
  rb_scan_args(argc,argv, "01", &scope);
  if (NIL_P(scope)) {
    return V8_Ref_Create(self, Context::New());
  } else {
    Local<ObjectTemplate> t = V8_Ref_Get<ObjectTemplate>(scope);    
    return V8_Ref_Create(self, Context::New(0, t));
  }
}

VALUE v8_cxt_Global(VALUE self) {
  HandleScope handles;
  convert_v8_to_rb_t v82rb;
  Local<Context> cxt = V8_Ref_Get<Context>(self);
  Local<Value> global = *cxt->Global();
  return v82rb(global);
}

VALUE v8_cxt_open(VALUE self) {
  HandleScope handles;
  TryCatch exceptions;
  Local<Context> cxt = V8_Ref_Get<Context>(self);
  Context::Scope enter(cxt);
  if (rb_block_given_p()) {
    printf("<div>About to execute Ruby Block</div>");
    VALUE result = rb_yield(self);
    printf("<div>Ruby Block was executed</div>\n");
    if (exceptions.HasCaught()) {
      return V8_Wrap_Message(exceptions.Message());
    } else {
      printf("<div>No exception</div>");
      return result;
    }
    return result;
  } else {
    printf("<div>No block given!</div>");
    return Qnil;
  }
}


