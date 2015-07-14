#include "rr.h"

namespace rr {

  void Object::Init() {
    ClassBuilder("Object", Value::Class).
      defineSingletonMethod("New", &New).

      defineMethod("Set", &Set).
      defineMethod("Get", &Get).
      defineMethod("Has", &Has).

      store(&Class);
  }

  VALUE Object::New(VALUE self, VALUE rb_isolate) {
    Isolate isolate(rb_isolate);
    Locker lock(isolate);

    return Object(isolate, v8::Object::New(isolate));
  }

  // TODO: Allow setting of property attributes
  VALUE Object::Set(VALUE self, VALUE key, VALUE value) {
    Object object(self);
    Locker lock(object.getIsolate());

    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return Bool(object->Set(UInt32(key), Value::rubyObjectToHandle(object.getIsolate(), value)));
    } else {
      return Bool(object->Set(*Value(key), Value::rubyObjectToHandle(object.getIsolate(), value)));
    }
  }

  VALUE Object::Get(VALUE self, VALUE key) {
    Object object(self);
    Locker lock(object.getIsolate());

    if (rb_obj_is_kind_of(key, rb_cNumeric)) {
      return Value::handleToRubyObject(object.getIsolate(), object->Get(UInt32(key)));
    } else {
      return Value::handleToRubyObject(object.getIsolate(), object->Get(*Value(key)));
    }
  }

  VALUE Object::Has(VALUE self, VALUE r_context, VALUE key) {
    Object object(self);
    Context context(r_context);
    Locker lock(object.getIsolate());
    v8::Maybe<bool> does_have(object->Has(context, *Value(key)));

    if (does_have.IsNothing()) {
      return Qnil;
    } else {
      return does_have.FromJust() ? Qtrue : Qfalse;
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
