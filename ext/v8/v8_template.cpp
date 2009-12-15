#include <ruby.h>
#include <v8.h>
#include "v8_ref.h"
#include "v8_template.h"
#include "converters.h"

using namespace v8;

Handle<Value> RubyInvocationCallback(const Arguments& args) {
  VALUE code = (VALUE)External::Unwrap(args.Data());
  if (NIL_P(code)) {
    return Null();
  } else {  
    convert_v8_to_rb_t arg_cvt;
    convert_rb_to_v8_t ret_cvt;
    
    VALUE* arguments = new VALUE[args.Length()];
    for(int c=0;c<args.Length(); ++c) {
      Handle<Value> val = args[c];
      arguments[c] = arg_cvt(val);
    }
      
    VALUE result = rb_funcall2(code, rb_intern("call"), args.Length(), arguments);
    delete [] arguments;
    
    Handle<Value> convertedResult = ret_cvt(result);
    return convertedResult  ;
  }
}
 
VALUE v8_Template_Set(VALUE self, VALUE name, VALUE value) {
  HandleScope handles;
  Local<Template> tmpl = V8_Ref_Get<Template>(self);
  Local<Data> data = V8_Ref_Get<Data>(value);
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
  return V8_Ref_Create(self,t,code);
}

