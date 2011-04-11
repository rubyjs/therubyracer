#include "v8_handle.h"
#include "v8_obj.h"
#include "v8_value.h"
#include "v8_template.h"
#include "v8_external.h"

using namespace v8;

#include <cstdio>

VALUE rr_cV8_C_Object;

namespace {
  
  Persistent<Object>& unwrap(VALUE object) {
    return rr_v8_handle<Object>(object);
  }
  
  VALUE Get(VALUE self, VALUE key) {
    HandleScope handles;
    Persistent<Object> obj(unwrap(self));
    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return rr_v82rb(obj->Get(NUM2UINT(key)));
    } else {
      return rr_v82rb(obj->Get(rr_rb2v8(key)->ToString()));
    }
  }
  
  VALUE New(VALUE rbclass) {
    HandleScope handles;
    if (!Context::InContext()) {
      rb_raise(rb_eScriptError, "Object::New() called without an entered Context");
      return Qnil;
    }
    return rr_v8_handle_new(rbclass, Object::New());
  }
  
  VALUE Set(VALUE self, VALUE key, VALUE value) {
    HandleScope handles;
    Persistent<Object> obj = unwrap(self);
    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return rr_v82rb(obj->Set(NUM2UINT(key), rr_rb2v8(value)));
    } else {
      return rr_v82rb(obj->Set(rr_rb2v8(key), rr_rb2v8(value)));
    }
  }

  VALUE GetPropertyNames(VALUE self) {
    HandleScope handles;
    Persistent<Object> object = unwrap(self);  
    Local<Value> names = object->GetPropertyNames();
    return rr_v82rb(names);
  }
  VALUE SetHiddenValue(VALUE self, VALUE key, VALUE value) {
    HandleScope scope;
    if (Context::InContext()) {
      unwrap(self)->SetHiddenValue(rr_rb2v8(key)->ToString(), rr_rb2v8(value));
    } else {
      rb_raise(rb_eScriptError, "Object::SetHiddenValue() called without an entered Context");
    }
    return Qnil;
  }
  VALUE GetHiddenValue(VALUE self, VALUE key) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->GetHiddenValue(rr_rb2v8(key)->ToString()));
  }
  VALUE GetPrototype(VALUE self) {
    HandleScope scope;
    return rr_v82rb(unwrap(self)->GetPrototype());
  }
  VALUE SetPrototype(VALUE self, VALUE prototype) {
    HandleScope scope;
    Handle<Value> proto(rr_rb2v8(prototype));
    return rr_v82rb(unwrap(self)->SetPrototype(rr_rb2v8(prototype)));
  }
}

void rr_init_obj() {
  rr_cV8_C_Object = rr_define_class("Object", rr_cV8_C_Value);
  rr_define_singleton_method(rr_cV8_C_Object, "New", New, 0);
  rr_define_method(rr_cV8_C_Object, "Get", Get, 1);
  rr_define_method(rr_cV8_C_Object, "Set", Set, 2);
  rr_define_method(rr_cV8_C_Object, "GetPropertyNames", GetPropertyNames, 0);
  rr_define_method(rr_cV8_C_Object, "GetHiddenValue", GetHiddenValue, 1);
  rr_define_method(rr_cV8_C_Object, "SetHiddenValue", SetHiddenValue, 2);
  rr_define_method(rr_cV8_C_Object, "GetPrototype", GetPrototype, 0);
  rr_define_method(rr_cV8_C_Object, "SetPrototype", SetPrototype, 1);
}

VALUE rr_reflect_v8_object(Handle<Value> value) {
  Local<Object> object(Object::Cast(*value));
  Local<Value> peer = object->GetHiddenValue(String::NewSymbol("TheRubyRacer::RubyObject"));
  return peer.IsEmpty() ? rr_v8_handle_new(rr_cV8_C_Object, object) : (VALUE)External::Unwrap(peer);
}

