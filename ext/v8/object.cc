#include "rr.h"

namespace rr {

  void Object::Init() {
    ClassBuilder("Object", Value::Class).
      defineSingletonMethod("New", &New).

      defineMethod("Set", &Set).
      defineMethod("Get", &Get).

      store(&Class);
  }

  VALUE Object::New(VALUE self) {
    return Object(v8::Object::New(v8::Isolate::GetCurrent()));
  }

  // TODO: Allow setting of property attributes
  VALUE Object::Set(VALUE self, VALUE key, VALUE value) {
    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return Bool(Object(self)->Set(UInt32(key), Value(value)));
    } else {
      return Bool(Object(self)->Set(*Value(key), Value(value)));
    }
  }

  VALUE Object::Get(VALUE self, VALUE key) {
    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return Value(Object(self)->Get(UInt32(key)));
    } else {
      return Value(Object(self)->Get(*Value(key)));
    }
  }

  Object::operator VALUE() {
    if (handle.IsEmpty()) {
      return Qnil;
    }

    Backref* backref;

    v8::Local<v8::String> key(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "rr::Backref"));
    v8::Local<v8::Value> external = handle->GetHiddenValue(key);

    VALUE value;

    if (external.IsEmpty()) {
      value = downcast();
      backref = new Backref(value);

      handle->SetHiddenValue(key, backref->toExternal());
    } else {
      v8::External* wrapper = v8::External::Cast(*external);
      backref = (Backref*)wrapper->Value();
      value = backref->get();

      if (!RTEST(value)) {
        value = downcast();
        backref->set(value);
      }
    }

    return value;
  }

  VALUE Object::downcast() {
    // TODO: Enable this when the methods are implemented
    // if (handle->IsFunction()) {
    //   return Function((v8::Handle<v8::Function>) v8::Function::Cast(*handle));
    // }
    //
    // if (handle->IsArray()) {
    //   return Array((v8::Handle<v8::Array>)v8::Array::Cast(*handle));
    // }
    //
    // if (handle->IsDate()) {
    //   // return Date(handle);
    // }
    //
    // if (handle->IsBooleanObject()) {
    //   // return BooleanObject(handle);
    // }
    //
    // if (handle->IsNumberObject()) {
    //   // return NumberObject(handle);
    // }
    //
    // if (handle->IsStringObject()) {
    //   // return StringObject(handle);
    // }
    //
    // if (handle->IsRegExp()) {
    //   // return RegExp(handle);
    // }

    return Ref<v8::Object>::operator VALUE();
  }
}
