#include "rr.h"

namespace rr {
  void Object::Init() {
    ClassBuilder("Object", Value::Class).
      defineSingletonMethod("New", &New).

      defineMethod("Set", &Set).
      defineMethod("Get", &Get).
      defineMethod("GetIdentityHash", &GetIdentityHash).
      defineMethod("Has", &Has).
      defineMethod("Delete", &Delete).
      defineMethod("SetAccessor", &SetAccessor).

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
    Locker lock(object);

    return INT2FIX(object->GetIdentityHash());
  }

  VALUE Object::Has(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Locker lock(object);

    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return Bool::Maybe(object->Has(Context(r_context), Uint32_t(key)));
    } else {
      return Bool::Maybe(object->Has(Context(r_context), *Value(key)));
    }
  }

  VALUE Object::Delete(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Locker lock(object);

    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return Bool::Maybe(object->Delete(Context(r_context), Uint32_t(key)));
    } else {
      return Bool::Maybe(object->Delete(Context(r_context), *Value(key)));
    }
  }

  VALUE Object::SetAccessor(int argc, VALUE* argv, VALUE self) {
    VALUE r_context, name, getter, setter, data, settings, attribute;
    rb_scan_args(argc, argv, "34", &r_context, &name, &getter, &setter, &data, &settings, &attribute);

    Object object(self);
    Context context(r_context);
    Isolate isolate(object.getIsolate());
    Locker lock(isolate);

    return Bool::Maybe(object->SetAccessor(
      context,
      Name(name),
      &PropertyCallbackInfo::invokeGetter,
      RTEST(setter) ? &PropertyCallbackInfoVoid::invokeSetter : 0,
      v8::MaybeLocal<v8::Value>(PropertyCallbackInfo::wrapData(isolate, getter, setter, data)),
      Enum<v8::AccessControl>(settings, v8::DEFAULT),
      Enum<v8::PropertyAttribute>(attribute, v8::None)
    ));
  }

  Object::operator VALUE() {
    Isolate isolate(getIsolate());
    Locker lock(isolate);

    if (handle->IsFunction()) {
      return Function(isolate, handle.As<v8::Function>());
    }

    // TODO: Enable this when the methods are implemented
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
