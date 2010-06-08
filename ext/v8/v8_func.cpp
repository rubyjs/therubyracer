
#include "v8_func.h"
#include "v8_obj.h"

using namespace v8;

namespace {
  VALUE FunctionClass;
  
  Local<Function> unwrap(VALUE value) {
    return V8_Ref_Get<Function>(value);
  }  
  VALUE Call(VALUE self, VALUE recv, VALUE arguments) {
    HandleScope handles;
    if (!Context::InContext()) {
      rb_raise(rb_eScriptError, "no open V8 Context in V8::C::Function::Call()");
      return Qnil;
    }
    Local<Function> function = unwrap(self);
    Local<Object> thisObj = rr_rb2v8(recv)->ToObject();
    Handle<Array> args = V8_Ref_Get<Array>(arguments);
    int argc = args->Length();
    Handle<Value> argv[argc];
    for (int i = 0; i < argc; i++) {
      argv[i] = args->Get(i);
    }
    return rr_v82rb(function->Call(thisObj, argc, argv));
  }
  
  VALUE NewInstance(VALUE self, VALUE arguments) {
    HandleScope scope;
    Local<Function> function = unwrap(self);
    Handle<Array> args = V8_Ref_Get<Array>(arguments);
    int argc = args->Length();
    Handle<Value> argv[argc];
    for (int i = 0; i < argc; i++) {
      argv[i] = args->Get(i);
    }
    return rr_v8_ref_create(rr_cV8_C_Object, function->NewInstance(argc, argv));
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
  rr_define_method(FunctionClass, "Call", Call, 2);
  rr_define_method(FunctionClass, "NewInstance", NewInstance, 1);
  rr_define_method(FunctionClass, "GetName", GetName, 0);
  rr_define_method(FunctionClass, "SetName", SetName, 1);
  // rr_define_method(FunctionClass, "GetScriptOrigin", GetScriptOrigin, 0);
}

VALUE rr_reflect_v8_function(Handle<Value> value) {
  Local<Function> function(Function::Cast(*value));
  return rr_v8_ref_create(FunctionClass, function);
}
