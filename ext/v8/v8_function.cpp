
#include <vector>
#include "v8_function.h"
#include "v8_object.h"
#include "v8_handle.h"

using namespace v8;

namespace {
  VALUE FunctionClass;
  
  Persistent<Function>& unwrap(VALUE value) {
    return rr_v8_handle<Function>(value);
  }  
  VALUE Call(VALUE self, VALUE recv, VALUE arguments) {
    HandleScope handles;
    if (!Context::InContext()) {
      rb_raise(rb_eScriptError, "no open V8 Context in V8::C::Function::Call()");
      return Qnil;
    }
    Handle<Function> function = unwrap(self);
    Local<Object> thisObj = rr_rb2v8(recv)->ToObject();
    Handle<Array> args = rr_v8_handle<Array>(arguments);
    int argc = args->Length();
    std::vector< Handle<Value> > argv (argc);
    for (int i = 0; i < argc; i++) {
      argv[i] = args->Get(i);
    }
    return rr_v82rb(function->Call(thisObj, argc, &argv[0]));
  }
  
  VALUE NewInstance(VALUE self, VALUE arguments) {
    HandleScope scope;
    Handle<Function> function = unwrap(self);
    Handle<Array> args = rr_v8_handle<Array>(arguments);
    int argc = args->Length();
    std::vector< Handle<Value> > argv (argc);
    for (int i = 0; i < argc; i++) {
      argv[i] = args->Get(i);
    }
    return rr_v82rb(function->NewInstance(argc, &argv[0]));
  }
  VALUE GetName(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->GetName());
  }
  VALUE SetName(VALUE self, VALUE name) {
    HandleScope scope;
    Handle<String> str = rr_v8_handle<String>(name);
    unwrap(self)->SetName(str);
    return Qnil;
  }  
  // VALUE GetScriptOrigin(VALUE self) {
  //   return rr_v82rb(unwrap(self)->GetScriptOrigin());
  // }
}

void rr_init_function() {
  FunctionClass = rr_define_class("Function", rr_v8_object_class());
  rr_define_method(FunctionClass, "Call", Call, 2);
  rr_define_method(FunctionClass, "NewInstance", NewInstance, 1);
  rr_define_method(FunctionClass, "GetName", GetName, 0);
  rr_define_method(FunctionClass, "SetName", SetName, 1);
  // rr_define_method(FunctionClass, "GetScriptOrigin", GetScriptOrigin, 0);
}

VALUE rr_reflect_v8_function(Handle<Value> value) {
  return rr_reflect_v8_object_as(value, FunctionClass);
}
