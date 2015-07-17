#include "rr.h"

namespace rr {

  void Object::Init() {
    ClassBuilder("Object", Value::Class).
      defineSingletonMethod("New", &New).

      defineMethod("Set", &Set).
      defineMethod("Get", &Get).

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

  Object::operator VALUE() {
    Locker lock(getIsolate());

    if (handle.IsEmpty()) {
      return Qnil;
    }

    Backref* backref;

    v8::Local<v8::String> key(v8::String::NewFromUtf8(getIsolate(), "rr::Backref"));
    v8::Local<v8::Value> external = handle->GetHiddenValue(key);

    VALUE value;

    if (external.IsEmpty()) {
      value = downcast();
      backref = new Backref(value);

      handle->SetHiddenValue(key, backref->toExternal(getIsolate()));
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
    Locker lock(getIsolate());

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
