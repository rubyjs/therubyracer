
#include "converters.h"
#include "v8_func.h"
#include "v8_obj.h"

using namespace v8;

VALUE rr_cV8_C_Function;

namespace {
  VALUE FunctionClass;
  
  Local<Function> unwrap(VALUE value) {
    return V8_Ref_Get<Function>(value);
  }  
  VALUE Call(int argc, VALUE *argv, VALUE self) {
    HandleScope handles;
    VALUE recv; VALUE f_argv;
    rb_scan_args(argc, argv, "1*", &recv, &f_argv);
    
    Local<Function> function = V8_Ref_Get<Function>(self);
    Local<Object> thisObject = V8_Ref_Get<Object>(recv);
    int f_argc = argc - 1;
    Local<Value> arguments[f_argc];
    for (int i = 0; i < f_argc; i++) {
      arguments[i] = *rr_rb2v8(rb_ary_entry(f_argv, i));
    }
    Local<Value> result = function->Call(thisObject, f_argc, arguments);
    return rr_v82rb(result);
  }
  VALUE NewInstance(int argc, VALUE *argv, VALUE self) {
    HandleScope handles;
    VALUE f_argv;
    rb_scan_args(argc, argv, "*", &f_argv);
    Local<Function> function = V8_Ref_Get<Function>(self);
    Local<Value> arguments[argc];
    for (int i = 0; i < argc; i++) {
      arguments[i] = *rr_rb2v8(rb_ary_entry(f_argv, i));
    }
    return rr_v82rb(function->NewInstance(argc, arguments));
  }
  VALUE GetName(VALUE self) {
    return rr_v82rb(unwrap(self)->GetName());
  }
  VALUE SetName(VALUE self, VALUE name) {
    Local<String> str = V8_Ref_Get<String>(name);
    unwrap(self)->SetName(str);
    return Qnil;
  }  
  // VALUE GetScriptOrigin(VALUE self) {
  //   return rr_v82rb(unwrap(self)->GetScriptOrigin());
  // }
}

void rr_init_func() {
  FunctionClass = rr_define_class("Function", rr_cV8_C_Object);
  rr_define_method(FunctionClass, "Call", Call, -1);
  rr_define_method(FunctionClass, "NewInstance", NewInstance, -1);
  rr_define_method(FunctionClass, "GetName", GetName, 0);
  rr_define_method(FunctionClass, "SetName", SetName, 1);
  // rr_define_method(FunctionClass, "GetScriptOrigin", GetScriptOrigin, 0);
}

VALUE rr_reflect_v8_function(Handle<Value> value) {
  Local<Function> f = Function::Cast(*value);
  return V8_Ref_Create(FunctionClass, f);
}
