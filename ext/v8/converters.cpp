#include "converters.h"
#include "callbacks.h"
#include "v8_ref.h"
#include "v8_obj.h"

using namespace v8;

namespace {
  std::string UNDEFINED_STR("undefined");
}

VALUE V8_To;

VALUE V82RB(Handle<Value>& value) {
  convert_v8_to_rb_t convert;
  VALUE result;
  if(convert(value, result)) {
    return result;
  }
  
  if (value->IsArray()) {  
    Local<Array> array(Array::Cast(*value));
    VALUE rb_array = rb_ary_new2(array->Length());
    for (unsigned int i = 0; i < array->Length(); i++) {
      Local<Value> value = array->Get(Integer::New(i));
      rb_ary_push(rb_array, V82RB(value));
    }
    return rb_array;
  }
  
  if (value->IsObject()) {
    Local<Object> object(Object::Cast(*value));
    return V8_Ref_Create(V8_C_Object, value);
  }
  
  return Qnil;
}

Local<Value> RB2V8(VALUE value) {  
  VALUE valueClass = rb_class_of(value);
  
  if(valueClass == rb_cProc || valueClass == rb_cMethod) {
    Local<FunctionTemplate> t = FunctionTemplate::New(RacerRubyInvocationCallback, External::Wrap((void *)value));
    return t->GetFunction();  
  }
  convert_rb_to_v8_t convert;
  Local<Value> result;
  if (convert(value, result)) {
    return result;
  }
  
  Local<ObjectTemplate> tmpl = ObjectTemplate::New();
  VALUE methods = rb_funcall(value, rb_intern("public_methods"), 1, Qfalse);
  int len = RARRAY_LEN(methods);
  for (int i = 0; i < len; i++) {
    VALUE method_name = RARRAY_PTR(methods)[i];
    VALUE camel_method_name = rb_funcall(V8_To, rb_intern("camelcase"), 1, method_name);
    VALUE method = rb_funcall(value, rb_intern("method"), 1, method_name);
    Local<String> keystr = (String *)*RB2V8(method_name);
    Local<String> camelstr = (String *)*RB2V8(camel_method_name);
    Local<Value> fun = RB2V8(method);
    tmpl->Set(keystr, fun);
    tmpl->Set(camelstr, fun);
  }
  return tmpl->NewInstance();
}

std::string V82String(Handle<Value>& value) {
  convert_v8_to_string_t convert;
  std::string result;
  if(convert(value, result)) {
    return result;
  }
  
  if (value->IsObject()) {
    Local<Object> object(Object::Cast(*value));
    Local<String> str = object->ToString();
    if(convert(value, result)) {
      return result;
    }
  }  
  
  return UNDEFINED_STR;
}