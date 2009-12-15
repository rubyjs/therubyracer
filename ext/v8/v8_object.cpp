#include "v8_object.h"
#include "v8_context.h"
#include "converters.h"
#include <stdio.h>

VALUE rb_cV8_JSObject;

using namespace v8;

v8_object::v8_object(VALUE clazz) {
  v8_context::ensure context;
  handle = (*Object::New());
  this->make_ruby_value(clazz);
}

v8_object::v8_object(Handle<Object>& object) : handle(Persistent<Object>(*object)) {
  Handle<Value> peer = object->GetHiddenValue(String::New("ruby::peer"));
  if (peer.IsEmpty()) {
    this->make_ruby_value(rb_cV8_JSObject);    
  } else {
    ruby_value = (VALUE)External::Unwrap(peer);
  }
}
v8_object::~v8_object() {  
  handle.Dispose();  
}

VALUE v8_object::make_ruby_value(VALUE clazz) {
  ruby_value = Data_Wrap_Struct(clazz, v8_object_mark, v8_object_free, this);
  v8_context::ensure context;
  handle->SetHiddenValue(String::New("ruby::peer"), External::Wrap((void *)ruby_value));
  return ruby_value;
}

VALUE v8_object_hash_access(VALUE self, VALUE key) {
  v8_object* object = 0;
  Data_Get_Struct(self, struct v8_object, object);
  
  convert_rb_to_string_t tostring;
  const std::string cppkey(tostring(key));
  HandleScope handles;
  Handle<Value> result = object->handle->Get(String::New(cppkey.c_str()));
  convert_v8_to_rb_t toValue;
  return toValue(result);
}

VALUE v8_object_hash_assignment(VALUE self, VALUE key, VALUE value) {
  
}

VALUE v8_object_call_something(VALUE self, VALUE code) {
  return rb_funcall(code, rb_intern("call"), 0);
}
  
VALUE v8_object_allocate(VALUE clazz) {
  v8_object *wrapper = new v8_object(clazz);
  return wrapper->ruby_value;
}

void v8_object_mark(v8_object *o) {
  
}
void v8_object_free(v8_object *o) {
  delete o;
}


