#include "v8.h"
#include "v8_ref.h"
#include "v8_script.h"
#include "v8_context.h"

#include "ruby_data.h"
#include "v8_data.h"

using namespace v8;


VALUE v8_script_new(VALUE self, VALUE source) {
  HandleScope handles;
  V8_Ref_Get(String, src, source);
  return V8_Ref_Create(self, Script::New(src));
}

VALUE v8_script_Run(VALUE self) {
  HandleScope handles;
  V8_Ref_Get(Script, script, self);
  V8HandleSource<RubyDest, VALUE> toValue;
  Local<Value> result = script->Run();
  return toValue.push(result);  
}