
#include "converters.h"
#include "v8_func.h"

using namespace v8;

VALUE V8_C_Function;

VALUE V8_Wrap_Function(Handle<Function> f) {
  return V8_Ref_Create(V8_C_Function, f);
}


VALUE v8_C_Function_Call(int argc, VALUE *argv, VALUE self) {
  HandleScope handles;
  VALUE recv; VALUE f_argv;
  rb_scan_args(argc, argv, "1*", &recv, &f_argv);

  Local<Function> function = V8_Ref_Get<Function>(self);
  Local<Object> thisObject(Object::Cast(*RB2V8(recv)));
  int f_argc = argc - 1;
  Local<Value> arguments[f_argc];
  for (int i = 0; i < f_argc; i++) {
    arguments[i] = RB2V8(rb_ary_entry(f_argv,i));
  }
  Local<Value> result = function->Call(thisObject, f_argc, arguments);
  return V82RB(result);
}