#include "ruby.h"
#include "v8.h"
#include "v8_ref.h"
#include "v8_template.h"

using namespace v8;

Handle<Value> RubyInvocationCallback(const Arguments& args) {
  VALUE code = (VALUE)External::Unwrap(args.Data());
  if (NIL_P(code)) {
    return String::New("Code was nil");
  } else {    
    VALUE result = rb_funcall(code, rb_intern("call"), 0);
    return String::New("Function Was Called");
  }
}
 
VALUE v8_Template_Set(VALUE self, VALUE name, VALUE value) {
  HandleScope handles;
  V8_Ref_Get(Template, tmpl, self);
  V8_Ref_Get(Data, data, value);
  tmpl->Set(RSTRING(name)->ptr, data);
  return Qnil;
}

VALUE v8_ObjectTemplate_New(VALUE clazz) {
  HandleScope handles;
  return V8_Ref_Create(clazz, ObjectTemplate::New());
}

VALUE v8_FunctionTemplate_New(int argc, VALUE *argv, VALUE self) {
  VALUE code;
  rb_scan_args(argc, argv, "00&", &code);
  HandleScope handles;
  Local<FunctionTemplate> t = FunctionTemplate::New(RubyInvocationCallback, External::Wrap((void *)code));
  return V8_Ref_Create2(self,t,code);
}

