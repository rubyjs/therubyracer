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
      defineMethod("SetAccessorProperty", &SetAccessorProperty).
      defineMethod("CreateDataProperty", &CreateDataProperty).
      defineMethod("DefineOwnProperty", &DefineOwnProperty).
      defineMethod("GetPropertyAttributes", &GetPropertyAttributes).
      defineMethod("GetOwnPropertyDescriptor", &GetOwnPropertyDescriptor).
      defineMethod("GetPropertyNames", &GetPropertyNames).
      defineMethod("GetOwnPropertyNames", &GetOwnPropertyNames).
      defineMethod("GetPrototype", &GetPrototype).
      defineMethod("SetPrototype", &SetPrototype).
      defineMethod("ObjectProtoToString", &ObjectProtoToString).
      defineMethod("GetConstructorName", &GetConstructorName).
      defineMethod("InternalFieldCount", &InternalFieldCount).
      defineMethod("GetInternalField", &GetInternalField).
      defineMethod("SetInternalField", &SetInternalField).
      defineMethod("HasOwnProperty", &HasOwnProperty).
      defineMethod("HasRealNamedProperty", &HasRealNamedProperty).
      defineMethod("HasRealIndexedProperty", &HasRealIndexedProperty).
      defineMethod("HasRealNamedCallbackProperty", &HasRealNamedCallbackProperty).
      defineMethod("GetRealNamedPropertyInPrototypeChain", &GetRealNamedPropertyInPrototypeChain).
      defineMethod("GetRealNamedProperty", &GetRealNamedProperty).
      defineMethod("GetRealNamedPropertyAttributes", &GetRealNamedPropertyAttributes).
      defineMethod("GetRealNamedPropertyAttributesInPrototypeChain", &GetRealNamedPropertyAttributesInPrototypeChain).
      defineMethod("HasNamedLookupInterceptor", &HasNamedLookupInterceptor).
      defineMethod("HasIndexedLookupInterceptor", &HasIndexedLookupInterceptor).
      defineMethod("SetHiddenValue", &SetHiddenValue).
      defineMethod("GetHiddenValue", &GetHiddenValue).
      defineMethod("DeleteHiddenValue", &DeleteHiddenValue).
      defineMethod("Clone", &Clone).
      defineMethod("CreationContext", &CreationContext).
      defineMethod("IsCallable", &IsCallable).
      defineMethod("CallAsFunction", &CallAsFunction).
      defineMethod("CallAsConstructor", &CallAsConstructor).

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
      &PropertyCallback::invokeGetter,
      RTEST(setter) ? &PropertyCallback::invokeSetter : 0,
      v8::MaybeLocal<v8::Value>(PropertyCallback::wrapData(isolate, getter, setter, data)),
      Enum<v8::AccessControl>(settings, v8::DEFAULT),
      Enum<v8::PropertyAttribute>(attribute, v8::None)
    ));
  }

  VALUE Object::SetAccessorProperty(int argc, VALUE* argv, VALUE self) {
    VALUE r_context, name, getter, setter, attribute, settings;
    rb_scan_args(argc, argv, "33", &r_context, &name, &getter, &setter, &attribute, &settings);

    Object object(self);
    Locker lock(object);

    object->SetAccessorProperty(
      Name(name),
      *Function(getter),
      RTEST(setter) ? *Function(setter) : v8::Local<v8::Function>(),
      Enum<v8::PropertyAttribute>(attribute, v8::None),
      Enum<v8::AccessControl>(settings, v8::DEFAULT)
    );

    return Qnil;
  }

  VALUE Object::CreateDataProperty(VALUE self, VALUE r_context, VALUE key, VALUE value) {
    Object object(self);
    Locker lock(object);

    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return Bool::Maybe(object->CreateDataProperty(Context(r_context), Uint32_t(key), Value(value)));
    } else {
      return Bool::Maybe(object->CreateDataProperty(Context(r_context), *Name(key), Value(value)));
    }
  }

  VALUE Object::DefineOwnProperty(int argc, VALUE* argv, VALUE self) {
    VALUE r_context, key, value, attribute;
    rb_scan_args(argc, argv, "31", &r_context, &key, &value, &attribute);

    Object object(self);
    Locker lock(object);

    return Bool::Maybe(object->DefineOwnProperty(
      Context(r_context),
      *Name(key),
      Value(value),
      Enum<v8::PropertyAttribute>(attribute, v8::None)
    ));
  }

  VALUE Object::GetPropertyAttributes(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Locker lock(object);

    return Enum<v8::PropertyAttribute>::Maybe(object->GetPropertyAttributes(
      Context(r_context),
      *Name(key)
    ));
  }

  VALUE Object::GetOwnPropertyDescriptor(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Locker lock(object);

    return Value::Maybe(object.getIsolate(), object->GetOwnPropertyDescriptor(
      Context(r_context),
      *rr::String(key)
    ));
  }

  VALUE Object::GetPropertyNames(VALUE self, VALUE r_context) {
    Object object(self);
    Locker lock(object);

    return Array::Maybe(object.getIsolate(), object->GetPropertyNames(Context(r_context)));
  }

  VALUE Object::GetOwnPropertyNames(VALUE self, VALUE r_context) {
    Object object(self);
    Locker lock(object);

    return Array::Maybe(object.getIsolate(), object->GetOwnPropertyNames(Context(r_context)));
  }

  VALUE Object::GetPrototype(VALUE self) {
    Object object(self);
    Locker lock(object);

    return Value(object.getIsolate(), object->GetPrototype());
  }

  VALUE Object::SetPrototype(VALUE self, VALUE r_context, VALUE prototype) {
    Object object(self);
    Locker lock(object);

    return Bool::Maybe(object->SetPrototype(
      Context(r_context),
      *Value(prototype)
    ));
  }

  VALUE Object::ObjectProtoToString(VALUE self, VALUE r_context) {
    Object object(self);
    Locker lock(object);

    return String::Maybe(object.getIsolate(), object->ObjectProtoToString(Context(r_context)));
  }

  VALUE Object::GetConstructorName(VALUE self) {
    Object object(self);
    Locker lock(object);

    return String(object.getIsolate(), object->GetConstructorName());
  }

  VALUE Object::InternalFieldCount(VALUE self) {
    Object object(self);
    Locker lock(object);

    return INT2FIX(object->InternalFieldCount());
  }

  VALUE Object::GetInternalField(VALUE self, VALUE index) {
    Object object(self);
    Locker lock(object);

    return Value(object.getIsolate(), object->GetInternalField(NUM2INT(index)));
  }

  VALUE Object::SetInternalField(VALUE self, VALUE index, VALUE value) {
    Object object(self);
    Locker lock(object);

    object->SetInternalField(NUM2INT(index), *Value(value));

    return Qnil;
  }

  VALUE Object::HasOwnProperty(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Locker lock(object);

    return Bool::Maybe(object->HasOwnProperty(Context(r_context), *Name(key)));
  }

  VALUE Object::HasRealNamedProperty(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Locker lock(object);

    return Bool::Maybe(object->HasRealNamedProperty(Context(r_context), *Name(key)));
  }

  VALUE Object::HasRealIndexedProperty(VALUE self, VALUE r_context, VALUE index) {
    Object object(self);
    Locker lock(object);

    return Bool::Maybe(object->HasRealIndexedProperty(Context(r_context), NUM2INT(index)));
  }

  VALUE Object::HasRealNamedCallbackProperty(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Locker lock(object);

    return Bool::Maybe(object->HasRealNamedCallbackProperty(Context(r_context), *Name(key)));
  }

  VALUE Object::GetRealNamedPropertyInPrototypeChain(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Locker lock(object);

    return Value::Maybe(object.getIsolate(), object->GetRealNamedPropertyInPrototypeChain(
      Context(r_context),
      *Name(key)
    ));
  }

  VALUE Object::GetRealNamedProperty(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Locker lock(object);

    return Value::Maybe(object.getIsolate(), object->GetRealNamedProperty(
      Context(r_context),
      *Name(key)
    ));
  }

  VALUE Object::GetRealNamedPropertyAttributes(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Locker lock(object);

    return Enum<v8::PropertyAttribute>::Maybe(object->GetRealNamedPropertyAttributes(
      Context(r_context),
      *Name(key)
    ));
  }

  VALUE Object::GetRealNamedPropertyAttributesInPrototypeChain(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Locker lock(object);

    return Enum<v8::PropertyAttribute>::Maybe(object->GetRealNamedPropertyAttributesInPrototypeChain(
      Context(r_context),
      *Name(key)
    ));
  }

  VALUE Object::HasNamedLookupInterceptor(VALUE self) {
    Object object(self);
    Locker lock(object);

    return Bool(object->HasNamedLookupInterceptor());
  }

  VALUE Object::HasIndexedLookupInterceptor(VALUE self) {
    Object object(self);
    Locker lock(object);

    return Bool(object->HasIndexedLookupInterceptor());
  }

  VALUE Object::SetHiddenValue(VALUE self, VALUE key, VALUE value) {
    Object object(self);
    Locker lock(object);

    return Bool(object->SetHiddenValue(String(key), Value(value)));
  }

  VALUE Object::GetHiddenValue(VALUE self, VALUE key) {
    Object object(self);
    Locker lock(object);

    return Value(object.getIsolate(), object->GetHiddenValue(String(key)));
  }

  VALUE Object::DeleteHiddenValue(VALUE self, VALUE key) {
    Object object(self);
    Locker lock(object);

    return Bool(object->DeleteHiddenValue(String(key)));
  }

  VALUE Object::Clone(VALUE self) {
    Object object(self);
    Locker lock(object);

    return Object(object.getIsolate(), object->Clone());
  }

  VALUE Object::CreationContext(VALUE self) {
    Object object(self);
    Locker lock(object);

    return Context(object->CreationContext());
  }

  VALUE Object::IsCallable(VALUE self) {
    Object object(self);
    Locker lock(object);

    return Bool(object->IsCallable());
  }

  VALUE Object::CallAsFunction(int argc, VALUE* argv, VALUE self) {
    VALUE r_context, recv, r_argv;
    rb_scan_args(argc, argv, "30", &r_context, &recv, &r_argv);

    Object object(self);
    v8::Isolate* isolate = object.getIsolate();
    Locker lock(isolate);

    std::vector< v8::Handle<v8::Value> > vector(Value::convertRubyArray(isolate, r_argv));

    return Value::Maybe(isolate, object->CallAsFunction(
      Context(r_context),
      Value(recv),
      RARRAY_LENINT(r_argv),
      &vector[0]
    ));
  }

  VALUE Object::CallAsConstructor(VALUE self, VALUE r_context, VALUE argv) {
    Object object(self);
    v8::Isolate* isolate = object.getIsolate();
    Locker lock(isolate);

    std::vector< v8::Handle<v8::Value> > vector(Value::convertRubyArray(isolate, argv));

    return Value::Maybe(isolate, object->CallAsConstructor(
      Context(r_context),
      RARRAY_LENINT(argv),
      &vector[0]
    ));
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
    if (handle->IsDate()) {
      return Date(isolate, handle.As<v8::Date>());
    }

    // TODO: Enable this when the methods are implemented
    //
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
