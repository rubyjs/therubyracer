
#include <ruby_data.h>
#include <v8_data.h>
#include <generic_data.h>
#include <v8_context.h>

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
  // printf("Before Script->Run()<br/>");
  Local<Value> result = script->Run();
  // printf("After Script->Run()<br/>");    
  V8HandleSource<RubyDest, VALUE> toValue;
  // printf("Result: %s", *String::AsciiValue(result->ToString()));
  return toValue.push(result);
  
  
}
