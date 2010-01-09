#include "v8.h"
#include "v8_ref.h"
#include "v8_script.h"
#include "converters.h"

using namespace v8;


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