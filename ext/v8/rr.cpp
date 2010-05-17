#include "rr.h"
#include "v8_cxt.h"
#include "v8_value.h"
#include "v8_obj.h"
#include "v8_func.h"
#include "v8_array.h"
#include "v8_str.h"

using namespace v8;

VALUE rr_define_class(const char *name, VALUE superclass) {
  VALUE V8 = rb_define_module("V8");
  VALUE V8_C = rb_define_module_under(V8, "C");
  return rb_define_class_under(V8_C, name, superclass);
}

VALUE rr_str_to_perl_case(VALUE str) {
  VALUE V8 = rb_define_module("V8");
  VALUE to = rb_define_module_under(V8, "To");
  return rb_funcall(to, rb_intern("perl_case"), 1, str);  
}

VALUE rr_str_to_camel_case(VALUE str) {
  VALUE V8 = rb_define_module("V8");
  VALUE to = rb_define_module_under(V8, "To");
  return rb_funcall(to, rb_intern("camel_case"), 1, str);
}

VALUE rr_v82rb(Handle<Value> value) {
  if (value->IsUndefined() || value->IsNull()) {
    return Qnil;
  }
  if (value->IsUint32()) {
    return UINT2NUM(value->Uint32Value());
  }
  if (value->IsInt32()) {
    return INT2FIX(value->Int32Value());
  }
  if (value->IsBoolean()) {
    return value->BooleanValue() ? true : false;
  }
  if (value->IsNumber()) {
    return rb_float_new(value->NumberValue());
  }  
  if (value->IsString()) {
    return rr_reflect_v8_string(value);
  }
  if (value->IsFunction()) {
    return rr_reflect_v8_function(value);
  }
  if (value->IsArray()) {
    return rr_reflect_v8_array(value);
  }
  if (value->IsObject()) {
    return rr_reflect_v8_object(value);
  }
  return rr_wrap_v8_value(value);  
}

VALUE rr_v82rb(Handle<Context> value) {
  return rr_reflect_v8_context(value);
}
VALUE rr_v82rb(Handle<Boolean> value) {
  return rr_v82rb((Handle<Value>)value);
}
VALUE rr_v82rb(Handle<Number> value) {
  return rr_v82rb((Handle<Value>)value);
}
VALUE rr_v82rb(Handle<String> value) {
  return rr_v82rb((Handle<Value>)value);
}
VALUE rr_v82rb(Handle<Object> value) {
  return rr_v82rb((Handle<Value>)value);
}
VALUE rr_v82rb(v8::Handle<v8::Function> value) {
  return rr_v82rb((Handle<Value>)value);
}
VALUE rr_v82rb(Handle<Integer> value) {
  return rr_v82rb((Handle<Value>)value);
}
VALUE rr_v82rb(Handle<Uint32> value) {
  return rr_v82rb((Handle<Value>)value);
}
VALUE rr_v82rb(Handle<Int32> value) {
  return rr_v82rb((Handle<Value>)value);
}
VALUE rr_v82rb(bool value) {
  return value ? Qtrue : Qfalse;
}
VALUE rr_v82rb(double value) {
  return rb_float_new(value);
}
VALUE rr_v82rb(int64_t value) {
  return INT2FIX(value);
}
VALUE rr_v82rb(uint32_t value) {
  return UINT2NUM(value);
}
VALUE rr_v82rb(int32_t value) {
  return INT2FIX(value);
}


Handle<Value> rr_rb2v8(VALUE value) {
  switch (TYPE(value)) {
  case T_FIXNUM:
    return Integer::New(FIX2INT(value));
  case T_FLOAT:
    return Number::New(NUM2DBL(value));    
  case T_STRING:
    return String::New(RSTRING_PTR(value), RSTRING_LEN(value));
  case T_NIL:
    return Null();
  case T_TRUE:
    return True();
  case T_FALSE:
    return False();
  }
  return Undefined();
}
// VALUE rr_v82rb(v8::ScriptData *data) {
//   return rr_thunk(rr_wrap_script_data(data));
// }



