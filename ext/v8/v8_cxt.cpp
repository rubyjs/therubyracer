#include "rr.h"
#include "v8_cxt.h"
#include "v8_msg.h"
#include "v8_template.h"
#include "converters.h"

using namespace v8;

VALUE V8_C_Context;

//TODO: rename everything to Context_
//TODO: do the object init from within here


namespace {
  
  Local<Context> unwrap(VALUE value) {
    return V8_Ref_Get<Context>(value);
  }
  
  VALUE New(int argc, VALUE *argv, VALUE self) {
    HandleScope handles;
    VALUE scope;
    rb_scan_args(argc,argv, "01", &scope);
    if (NIL_P(scope)) {
      Persistent<Context> cxt(Context::New());
      VALUE ref = V8_Ref_Create(self, cxt);
      cxt.Dispose();
      return ref;
    } else {
      Persistent<Context> context = Context::New(0, Racer_Create_V8_ObjectTemplate(scope));
      Context::Scope enter(context);
      context->Global()->SetHiddenValue(String::New("TheRubyRacer::RubyObject"), External::Wrap((void *)scope));
      VALUE ref = V8_Ref_Create(self, context, scope);
      context.Dispose();
      return ref;
    }
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
    unwrap(self)->Enter();
    return self;
  }
  
  VALUE Exit(VALUE self) {
    unwrap(self)->Exit();
    return self;
  }
}

void rr_init_cxt() {
  VALUE Context = V8_C_Context = rr_define_class("Context");
  rr_define_singleton_method(Context, "new", New, -1);
  rr_define_singleton_method(Context, "New", New, -1);
  rr_define_singleton_method(Context, "InContext", InContext, 0);
  rr_define_singleton_method(Context, "GetEntered", GetEntered, 0);
  rr_define_method(Context, "Global", Global, 0);
  rr_define_method(Context, "Enter", Enter, 0);
  rr_define_method(Context, "Exit", Exit, 0);
}

VALUE rr_reflect_v8_context(Handle<Context> value) {
  return V8_Ref_Create(V8_C_Context, value);
}


