#include "v8.h"
#include "v8_ref.h"
#include "v8_script.h"
#include "converters.h"

using namespace v8;

void rr_init_script() {
  VALUE Script = rr_define_class("Script");
  rr_define_singleton_method(Script, "new", v8_script_new, 1);
  rr_define_method(Script, "Run", v8_script_Run, 0);  
}

VALUE v8_script_new(VALUE self, VALUE source) {
  HandleScope handles;
  Local<String> src = V8_Ref_Get<String>(source);
  return V8_Ref_Create(self, Script::New(src));
}

VALUE v8_script_Run(VALUE self) {
  HandleScope handles;
  Local<Script> script = V8_Ref_Get<Script>(self);
  Local<Value> result = script->Run();
  return V82RB(result);
}