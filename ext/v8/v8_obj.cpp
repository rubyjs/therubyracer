#include "v8_obj.h"
#include "v8_ref.h"
#include "v8_value.h"
#include "v8_template.h"
#include "converters.h"

using namespace v8;

#include <cstdio>

VALUE rr_cV8_C_Object;

namespace {
  
  Local<Object> unwrap(VALUE robj) {
    return V8_Ref_Get<Object>(robj);
  }
  
  VALUE Get(VALUE self, VALUE key) {
    HandleScope handles;
    Local<Object> obj(unwrap(self));
    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return rr_v82rb(obj->Get(NUM2UINT(key)));
    } else {
      return rr_v82rb(obj->Get(rr_rb2v8(key)->ToString()));
    }
  }
  
  VALUE New(VALUE clazz) {
    HandleScope handles;
    if (!Context::InContext()) {
      rb_raise(rb_eScriptError, "Object::New() called without an entered Context");
      return Qnil;
    }
    return V8_Ref_Create(clazz, Object::New());
  }
  
  VALUE Set(VALUE self, VALUE key, VALUE value) {
    HandleScope handles;
    Local<Object> obj = unwrap(self);
    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return rr_v82rb(obj->Set(NUM2UINT(key), rr_rb2v8(value)));
    } else {
      return rr_v82rb(obj->Set(rr_rb2v8(key), rr_rb2v8(value)));
    }
  }

  VALUE GetPropertyNames(VALUE self) {
    HandleScope handles;
    Local<Object> object = unwrap(self);  
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
    //TODO: need to store a reference here? what's the best way
    // rr_v8_ref_setref(self, "RubyPeer", )
    return Qnil;
  }
}

void rr_init_obj() {
  rr_cV8_C_Object = rr_define_class("Object", rr_cV8_C_Value);
  rb_define_attr(rr_cV8_C_Object, "context", 1, 0);
  rr_define_singleton_method(rr_cV8_C_Object, "New", New, 0);
  rr_define_method(rr_cV8_C_Object, "Get", Get, 1);
  rr_define_method(rr_cV8_C_Object, "Set", Set, 2);
  rr_define_method(rr_cV8_C_Object, "GetPropertyNames", GetPropertyNames, 0);
  rr_define_method(rr_cV8_C_Object, "SetHiddenValue", SetHiddenValue, 2);
}

VALUE rr_reflect_v8_object(Handle<Value> value) {
  Local<Object> object(Object::Cast(*value));
  Local<Value> peer = object->GetHiddenValue(String::New("TheRubyRacer::RubyObject"));
  if (peer.IsEmpty()) {
    VALUE obj = V8_Ref_Create(rr_cV8_C_Object, object);
    rb_iv_set(obj, "@context", rr_v82rb(Context::GetEntered()));
    return obj;
  } else {
    return (VALUE)External::Unwrap(peer);
  }
}


v8::Handle<v8::Value> rr_reflect_rb_object(VALUE value) {
  Local<Object> o = Racer_Create_V8_ObjectTemplate(value)->NewInstance();
  o->SetHiddenValue(String::New("TheRubyRacer::RubyObject"), External::Wrap((void *) value));
  return o;  
}
