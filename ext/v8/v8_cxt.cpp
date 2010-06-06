#include "rr.h"
#include "v8_cxt.h"
#include "v8_msg.h"
#include "v8_template.h"
#include "v8_external.h"

using namespace v8;

VALUE V8_C_Context;

namespace {
  
  Local<Context> unwrap(VALUE value) {
    return V8_Ref_Get<Context>(value);
  }

  VALUE New(int argc, VALUE *argv, VALUE self) {
    HandleScope handles;
    VALUE global_template; VALUE global_object;
    rb_scan_args(argc,argv, "02", &global_template, &global_object);
    Handle<ObjectTemplate> v8_global_template(NIL_P(global_template) ? Handle<ObjectTemplate>() : V8_Ref_Get<ObjectTemplate>(global_template));
    Handle<Value> v8_global_object(NIL_P(global_object) ? Handle<Value>() : V8_Ref_Get<Value>(global_object));
    Persistent<Context> cxt(Context::New(0, v8_global_template, v8_global_object));
    VALUE ref = V8_Ref_Create(self, cxt);
    cxt.Dispose();
    return ref;
  }

  VALUE InContext(VALUE self) {
    return Context::InContext() ? Qtrue : Qfalse;  
  }
  
  VALUE GetEntered(VALUE self) {
    HandleScope handles;
    if (Context::InContext()) {
      Local<Context> current = Context::GetEntered();
      return V8_Ref_Create(self, current);    
    } else {
      return Qnil;
    }
  }
  
  VALUE Global(VALUE self) {
    HandleScope handles;
    return rr_v82rb(unwrap(self)->Global());
  }  
  
  VALUE Enter(VALUE self) {
    HandleScope handles;
    unwrap(self)->Enter();
    return self;
  }
  
  VALUE Exit(VALUE self) {
    HandleScope handles;
    unwrap(self)->Exit();
    return self;
  }
  
  VALUE IsEntered(VALUE self) {
    HandleScope handles;
    if (Context::InContext()) {
      return rr_v82rb(unwrap(self) == Context::GetEntered());
    } else {
      return Qfalse;
    }
  }
}

void rr_init_cxt() {
  VALUE ContextClass = V8_C_Context = rr_define_class("Context");
  rr_define_singleton_method(ContextClass, "New", New, -1);
  rr_define_singleton_method(ContextClass, "InContext", InContext, 0);
  rr_define_singleton_method(ContextClass, "GetEntered", GetEntered, 0);
  rr_define_method(ContextClass, "Global", Global, 0);
  rr_define_method(ContextClass, "Enter", Enter, 0);
  rr_define_method(ContextClass, "Exit", Exit, 0);
  rr_define_method(ContextClass, "IsEntered", IsEntered, 0);
}

VALUE rr_reflect_v8_context(Handle<Context> value) {
  return V8_Ref_Create(V8_C_Context, value);
}


