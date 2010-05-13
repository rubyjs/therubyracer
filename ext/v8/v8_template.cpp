#include <ruby.h>
#include <v8.h>
#include "v8_ref.h"
#include "v8_func.h"
#include "v8_template.h"
#include "converters.h"
#include "callbacks.h"

using namespace v8;


void rr_init_template() {
  VALUE Template = rr_define_class("Template");
  rb_define_method(Template, "Set", (VALUE(*)(...))v8_Template_Set, 2);
  
  VALUE ObjectTemplate = rr_define_class("ObjectTemplate", Template);
  rb_define_singleton_method(ObjectTemplate, "new", (VALUE(*)(...))v8_ObjectTemplate_New, 0);
  
  VALUE FunctionTemplate = rr_define_class("FunctionTemplate", Template);
  rb_define_singleton_method(FunctionTemplate, "new", (VALUE(*)(...))v8_FunctionTemplate_New, -1);
  rb_define_method(FunctionTemplate, "GetFunction", (VALUE(*)(...))v8_FunctionTemplate_GetFunction, 0);
  
}

Local<ObjectTemplate> Racer_Create_V8_ObjectTemplate(VALUE value) {
  Local<ObjectTemplate> tmpl = ObjectTemplate::New();
  // tmpl->SetInternalFieldCount(2);
  tmpl->SetNamedPropertyHandler(
    RacerRubyNamedPropertyGetter,
    RacerRubyNamedPropertySetter,
    0, // RacerRubyNamedPropertyQuery,
    0, // RacerRubyNamedPropertyDeleter,
    RacerRubyNamedPropertyEnumerator,
    External::Wrap((void *)value)
  );
  return tmpl;
}


 
VALUE v8_Template_Set(VALUE self, VALUE name, VALUE value) {
  HandleScope handles;
  Local<Template> tmpl = V8_Ref_Get<Template>(self);
  Local<Data> data = V8_Ref_Get<Data>(value);
  tmpl->Set(RSTRING_PTR(name), data);
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
  Local<FunctionTemplate> t = FunctionTemplate::New(RacerRubyInvocationCallback, External::Wrap((void *)code));
  return V8_Ref_Create(self,t,code);
}

VALUE v8_FunctionTemplate_GetFunction(VALUE self) {
  HandleScope handles;
  Local<FunctionTemplate> t = V8_Ref_Get<FunctionTemplate>(self);
  return rr_v82rb(t->GetFunction());
}

