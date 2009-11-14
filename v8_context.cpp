#include "ruby_data.h"
#include "v8_data.h"
#include "generic_data.h"
#include "v8_context.h"

#include<stdio.h>

using namespace v8;

v8_context::v8_context() : handle(Context::New()) {}

v8_context::~v8_context() {
  handle.Dispose();
}

VALUE v8_context_allocate(VALUE clazz) {
  v8_context *cxt = new v8_context;
  return Data_Wrap_Struct(clazz, v8_context_mark, v8_context_free, cxt);
  
}
void v8_context_free(v8_context *context) {
  delete context;
}
void v8_context_mark(v8_context *context) {
  //don't mark anything.
}


//methods
VALUE v8_context_eval(VALUE self, VALUE javascript) {
  v8_context* cxt = 0;
  Data_Get_Struct(self, struct v8_context, cxt);
  
  Context::Scope enter(cxt->handle);
  HandleScope handles;
  
  RubyValueSource<StringDest, std::string> tostring;
  const std::string source(tostring.push(javascript));
  
  Local<Script> script = Script::Compile(String::New(source.c_str()));
  Local<Value> result = script->Run();
  V8HandleSource<RubyDest, VALUE> toValue;
  return toValue.push(result);
}

VALUE v8_context_inject(VALUE self, VALUE key, VALUE value) {
    
    v8_context* context = 0;
    Data_Get_Struct(self, struct v8_context, context);
    
    Context::Scope enter(context->handle);
    HandleScope handles;
    
    RubyValueSource<StringDest, std::string> tostring;
    RubyValueSource<V8HandleDest, Persistent<Value> > toHandle;

    const std::string key_string(tostring.push(key));
    const std::string value_string(tostring.push(value));
        
    // does this need to be a persistent handle ?
    Persistent<Value> key_handle(String::New(key_string.c_str()));
    Persistent<Value> value_handle(toHandle.push(value));

    Local<Object> global = context->handle->Global();
    global->Set(key_handle, value_handle);

    return Qnil;
}

