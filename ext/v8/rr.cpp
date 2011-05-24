#include "rr.h"
#include "v8_context.h"
#include "v8_handle.h"
#include "v8_value.h"
#include "v8_object.h"
#include "v8_function.h"
#include "v8_array.h"
#include "v8_string.h"
#include "v8_date.h"
#include "v8_message.h"
#include "v8_external.h"
#include "v8_exception.h"

using namespace v8;

VALUE rr_define_class(const char *name, VALUE superclass) {
  VALUE V8 = rb_define_module("V8");
  VALUE V8_C = rb_define_module_under(V8, "C");
  VALUE klass = rb_define_class_under(V8_C, name, superclass);
  rb_funcall(klass, rb_intern("private_class_method"), 1, rb_str_new2("new"));
  return klass;
}

VALUE rr_define_module(const char *name) {
  VALUE V8 = rb_define_module("V8");
  VALUE V8_C = rb_define_module_under(V8, "C");
  return rb_define_module_under(V8_C, name);
}

VALUE rr_define_const(const char *name, VALUE value) {
  VALUE V8 = rb_define_module("V8");
  VALUE V8_C = rb_define_module_under(V8, "C");
  rb_define_const(V8_C, name, value);
  return value;
}

VALUE rr_const_get(const char *name) {
  VALUE V8 = rb_define_module("V8");
  VALUE V8_C = rb_define_module_under(V8, "C");
  return rb_const_get(V8_C, rb_intern(name));
}

VALUE rr_define_finalizer(VALUE object, void* finalizer, VALUE data) {
  VALUE finalizer_proc = rb_proc_new((VALUE (*)(...))finalizer, data);
  rb_iv_set(finalizer_proc, "data", data);
  VALUE ospace = rb_const_get(rb_cObject, rb_intern("ObjectSpace"));
  rb_funcall(ospace, rb_intern("define_finalizer"), 2, object, finalizer_proc);
}

VALUE rr_v82rb(Handle<Value> value) {
  if (value.IsEmpty()) {
    return rr_v8_value_empty();
  }
  if (value->IsUndefined() || value->IsNull()) {
    return Qnil;
  }
  if (value->IsExternal()) {
    return rr_reflect_v8_external(value);
  }
  if (value->IsUint32()) {
    return UINT2NUM(value->Uint32Value());
  }
  if (value->IsInt32()) {
    return INT2FIX(value->Int32Value());
  }
  if (value->IsBoolean()) {
    return value->BooleanValue() ? Qtrue : Qfalse;
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
  if (value->IsDate()) {
    return rr_reflect_v8_date(value);
  }
  if (value->IsObject()) {
    return rr_reflect_v8_object(value);
  }
  return rr_wrap_v8_value(value);  
}

VALUE rr_v82rb(Handle<Message> value) {
  return rr_reflect_v8_message(value);
}
VALUE rr_v82rb(Handle<StackTrace> value) {
  return rr_reflect_v8_stacktrace(value);
}
VALUE rr_v82rb(Handle<StackFrame> value) {
  return rr_reflect_v8_stackframe(value);
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
VALUE rr_v82rb(Handle<Array> value) {
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
  return LONG2NUM(value);
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
    // TODO: use this conversion if value will fit in 32 bits.
    // return Integer::New(FIX2LONG(value));
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
  case T_DATA:
    return rr_v8_handle<Value>(value);
  case T_OBJECT:
  case T_CLASS:
  case T_ICLASS:
  case T_MODULE:
  case T_REGEXP:
  case T_MATCH:
  case T_ARRAY:
  case T_HASH:
  case T_STRUCT:
  case T_BIGNUM:
  case T_FILE:
  case T_SYMBOL:
//  case T_BLKTAG: (not in 1.9)
  case T_UNDEF:
//  case T_VARMAP: (not in 1.9)
//  case T_SCOPE: (not in 1.9)
  case T_NODE:  
  default:
    rb_warn("unknown conversion to V8 for: %s", RSTRING_PTR(rb_inspect(value)));
    return String::New("Undefined Conversion");
  }

  return Undefined();
}
// VALUE rr_v82rb(v8::ScriptData *data) {
//   return rr_thunk(rr_wrap_script_data(data));
// }



