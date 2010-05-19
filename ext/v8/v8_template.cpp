#include <ruby.h>
#include <v8.h>
#include "v8_ref.h"
#include "v8_func.h"
#include "v8_template.h"
#include "v8_callbacks.h"
#include "callbacks.h"
#include "converters.h"

using namespace v8;

namespace {
  Local<Template> tmpl(VALUE self) {
    return V8_Ref_Get<Template>(self);
  }
  Local<ObjectTemplate> obj(VALUE self) {
    return V8_Ref_Get<ObjectTemplate>(self);
  }
  Local<FunctionTemplate> func(VALUE self) {
    return V8_Ref_Get<FunctionTemplate>(self);
  }

  VALUE Set(VALUE self, VALUE name, VALUE value) {
    HandleScope handles;
    Local<String> key = rr_rb2v8(name)->ToString();
    Local<Data> data = V8_Ref_Get<Data>(value);
    tmpl(self)->Set(key, data);
    return Qnil;
  }

  namespace Obj {

  }

  namespace Func {
    VALUE New(VALUE function_template) {
      HandleScope handles;
      rb_need_block();
      VALUE code = rb_block_proc();
      if (NIL_P(code)) {
        return Qnil;
      }
      Local<FunctionTemplate> templ = FunctionTemplate::New(RubyInvocationCallback, External::Wrap((void *)code));
      return V8_Ref_Create(function_template,templ,code);
    }
    VALUE GetFunction(VALUE self) {
      if (!Context::InContext()) {
        rb_raise(rb_eScriptError, "calls to FunctionTemplate::GetFunction() require a Context to be entered");
        return Qnil;
      }
      HandleScope handles;
      Local<FunctionTemplate> templ = func(self);
      Local<Function> fun = templ->GetFunction();
      return rr_v82rb(func(self)->GetFunction());
    }
  }
}

void rr_init_template() {
  VALUE Template = rr_define_class("Template");
  rr_define_method(Template, "Set", Set, 2);
  
  VALUE ObjectTemplate = rr_define_class("ObjectTemplate", Template);
  rb_define_singleton_method(ObjectTemplate, "new", (VALUE(*)(...))v8_ObjectTemplate_New, 0);
  
  VALUE FunctionTemplate = rr_define_class("FunctionTemplate", Template);
  rr_define_singleton_method(FunctionTemplate, "New", Func::New, 0);
  // rb_define_singleton_method(FunctionTemplate, "new", (VALUE(*)(...))v8_FunctionTemplate_New, -1);
  rr_define_method(FunctionTemplate, "GetFunction", Func::GetFunction, 0);
  
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

