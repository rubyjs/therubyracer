#include "ruby.h"
#include "v8.h"
#include "v8_ref.h"
#include "v8_template.h"

using namespace v8;

Handle<Value> RubyInvocationCallback(const Arguments& args) {
  Local<Value> data = args.Data();
  return data->ToString();
}
 
VALUE v8_template_Set(VALUE self, VALUE name, VALUE value) {
  HandleScope handles;
  V8_Ref_Get(Template, tmpl, self);
  V8_Ref_Get(Data, data, value);
  tmpl->Set(RSTRING(name)->ptr, data);
  return Qnil;
}

VALUE v8_FunctionTemplate_New(VALUE clazz) {
  HandleScope handles;
  Local<FunctionTemplate> t = FunctionTemplate::New(RubyInvocationCallback, String::New("This is some data!"));
  return V8_Ref_Create(clazz,t);
}

