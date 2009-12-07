#include "v8.h"
#include "v8_ref.h"
#include "v8_script.h"
#include "v8_context.h"

#include "ruby_data.h"
#include "v8_data.h"

using namespace v8;

Local<Script> VALUE_TO_SCRIPT(VALUE value) {
  v8_ref* ref = 0;
  Data_Get_Struct(value, struct v8_ref, ref);
  return (Script *)(*ref->handle);
}

VALUE v8_script_new(VALUE self, VALUE source) {
  v8_ref* src_ref = 0;
  Data_Get_Struct(source, struct v8_ref, src_ref);
  HandleScope handles;

  Local<String> src((String *)*src_ref->handle);
  v8_ref* script_ref = new v8_ref(Script::New(src));
  return Data_Wrap_Struct(self, v8_ref_mark, v8_ref_free, script_ref);
}

VALUE v8_script_Run(VALUE self) {
  HandleScope handles;
  Local<Script> script = VALUE_TO_SCRIPT(self);
  V8HandleSource<RubyDest, VALUE> toValue;
  Local<Value> result = script->Run();
  return toValue.push(result);  
}