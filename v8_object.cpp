#include <v8_object.h>
#include <ruby_data.h>
#include <v8_data.h>
#include <generic_data.h>

#include <stdio.h>

VALUE rb_cV8_JSObject;

using namespace v8;

v8_object::v8_object() {
  Persistent<Context> context = Context::InContext() ? *Context::GetCurrent() : Context::New();
  Context::Scope cscope(context);
  HandleScope hscope;
  handle = (*Object::New());
  context.Dispose();
}

v8_object::v8_object(Handle<Object>& object) : handle(Persistent<Object>(*object)) {
  
}
v8_object::~v8_object() {  
  handle.Dispose();  
}

VALUE v8_object_hash_access(VALUE self, VALUE key) {
  v8_object* object = 0;
  Data_Get_Struct(self, struct v8_object, object);
  
  RubyValueSource<StringDest, std::string> tostring;
  const std::string cppkey(tostring.push(key));
  
  HandleScope scope;
  Handle<Value> result = object->handle->Get(String::New(cppkey.c_str()));
  
  V8HandleSource<RubyDest, VALUE> toValue;
  return toValue.push(result);
}
  
VALUE v8_object_allocate(VALUE clazz) {
  v8_object *wrapper = new v8_object;
  return Data_Wrap_Struct(clazz, v8_object_mark, v8_object_free, wrapper);
}

void v8_object_mark(v8_object *o) {
  
}
void v8_object_free(v8_object *o) {
  delete o;
}


