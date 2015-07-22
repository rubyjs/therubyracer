#include "rr.h"

namespace rr {

  void Object::Init() {
    ClassBuilder("Object", Value::Class).
      defineSingletonMethod("New", &New).

      defineMethod("Set", &Set).
      defineMethod("Get", &Get).
      defineMethod("GetIdentityHash", &GetIdentityHash).

      store(&Class);
  }

  VALUE Object::New(VALUE self, VALUE rb_isolate) {
    Isolate isolate(rb_isolate);
    Locker lock(isolate);

    return Object(isolate, v8::Object::New(isolate));
  }

  // TODO: Allow setting of property attributes
  VALUE Object::Set(VALUE self, VALUE r_context, VALUE key, VALUE value) {
    Object object(self);
    Context context(r_context);
    Isolate isolate(object.getIsolate());
    Locker lock(isolate);

    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return Bool::Maybe(object->Set(context, Uint32_t(key), Value::rubyObjectToHandle(isolate, value)));
    } else {
      return Bool::Maybe(object->Set(context, *Value(key), Value::rubyObjectToHandle(isolate, value)));
    }
  }

  VALUE Object::Get(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Context context(r_context);
    Isolate isolate(object.getIsolate());
    Locker lock(isolate);

    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return Value::Maybe(isolate, object->Get(context, Uint32_t(key)));
    } else {
      return Value::Maybe(isolate, object->Get(context, *Value(key)));
    }
  }

  VALUE Object::GetIdentityHash(VALUE self) {
    Object object(self);
    Locker lock(object.getIsolate());

    return INT2FIX(object->GetIdentityHash());
  }

  Object::operator VALUE() {
    Isolate isolate(getIsolate());
    Locker lock(isolate);

    if (handle->IsFunction()) {
      return Function(isolate, handle.As<v8::Function>());
    }

    if (handle->IsArray()) {
      return Array(isolate, handle.As<v8::Array>());
    }

    // TODO: Enable this when the methods are implemented
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
