#include "v8_handle.h"
#include "v8_weakref.h"
#include "v8_object.h"
#include "v8_value.h"
#include "v8_template.h"
#include "v8_external.h"

using namespace v8;

#include <cstdio>

namespace {

  VALUE ObjectClass;

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
  VALUE GetIdentityHash(VALUE self) {
    return rr_v82rb(unwrap(self)->GetIdentityHash());
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

VALUE rr_v8_object_class() {
  return ObjectClass;
}

void rr_init_object() {
  ObjectClass = rr_define_class("Object", rr_v8_value_class());
  rr_define_singleton_method(ObjectClass, "New", New, 0);
  rr_define_method(ObjectClass, "Get", Get, 1);
  rr_define_method(ObjectClass, "Set", Set, 2);
  rr_define_method(ObjectClass, "GetPropertyNames", GetPropertyNames, 0);
  rr_define_method(ObjectClass, "GetIdentityHash", GetIdentityHash, 0);
  rr_define_method(ObjectClass, "GetHiddenValue", GetHiddenValue, 1);
  rr_define_method(ObjectClass, "SetHiddenValue", SetHiddenValue, 2);
  rr_define_method(ObjectClass, "GetPrototype", GetPrototype, 0);
  rr_define_method(ObjectClass, "SetPrototype", SetPrototype, 1);
}

VALUE rr_reflect_v8_object_as(Handle<Value> value, VALUE ruby_class) {
  Handle<Object> object = Handle<Object>::Cast(value);
  VALUE handle;
  v8_weakref* backref;
  Local<Value> holder = object->GetHiddenValue(String::NewSymbol("TheRubyRacer::Backref"));
  if (holder.IsEmpty()) {
    handle = rr_v8_handle_new(ruby_class, object);
    backref = new v8_weakref(handle);
    object->SetHiddenValue(String::NewSymbol("TheRubyRacer::Backref"), backref->external);
  } else {
    backref = (v8_weakref*)External::Unwrap(holder);
    handle = backref->get();
    if (!RTEST(handle)) {
      handle = rr_v8_handle_new(ruby_class, object);
      backref->set(handle);
    }
  }
  return handle;
}

VALUE rr_reflect_v8_object(Handle<Value> value) {
  return rr_reflect_v8_object_as(value, ObjectClass);
}

