#include "rr.h"

namespace rr {

  v8::Local<v8::Value> PropertyCallback::wrapData(v8::Isolate* isolate, VALUE r_getter, VALUE r_setter,
                                                  VALUE r_query, VALUE r_deleter, VALUE r_enumerator,
                                                  VALUE r_data) {
    v8::Local<v8::Object> holder = v8::Object::New(isolate);

    v8::Local<v8::String> getter_key = v8::String::NewFromUtf8(isolate, "rr::getter");
    holder->SetHiddenValue(getter_key, External::wrap(isolate, r_getter));

    v8::Local<v8::String> setter_key = v8::String::NewFromUtf8(isolate, "rr::setter");
    holder->SetHiddenValue(setter_key, External::wrap(isolate, r_setter));

    v8::Local<v8::String> data_key = v8::String::NewFromUtf8(isolate, "rr::data");
    holder->SetHiddenValue(data_key, rr::Value(r_data));

    if (RTEST(r_query)) {
      v8::Local<v8::String> query_key = v8::String::NewFromUtf8(isolate, "rr::query");
      holder->SetHiddenValue(query_key, rr::Value(r_query));
    }

    if (RTEST(r_deleter)) {
      v8::Local<v8::String> deleter_key = v8::String::NewFromUtf8(isolate, "rr::deleter");
      holder->SetHiddenValue(deleter_key, rr::Value(r_deleter));
    }

    if (RTEST(r_enumerator)) {
      v8::Local<v8::String> enumerator_key = v8::String::NewFromUtf8(isolate, "rr::enumerator");
      holder->SetHiddenValue(enumerator_key, rr::Value(r_enumerator));
    }

    return holder;
  }

  v8::Local<v8::Value> PropertyCallback::wrapData(v8::Isolate* isolate, VALUE r_getter, VALUE r_setter, VALUE r_data) {
    return wrapData(isolate, r_getter, r_setter, Qnil, Qnil, Qnil, r_data);
  }

  VALUE PropertyCallback::unwrapData(v8::Isolate* isolate, v8::Local<v8::Value> value) {
    v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(value);
    v8::Local<v8::String> data_key = v8::String::NewFromUtf8(isolate, "rr::data");

    return Value(isolate, holder->GetHiddenValue(data_key));
  }

  VALUE PropertyCallback::unwrapGetter(v8::Isolate* isolate, v8::Local<v8::Value> value) {
    v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(value);
    v8::Local<v8::String> callback_key = v8::String::NewFromUtf8(isolate, "rr::getter");

    return External::unwrap(holder->GetHiddenValue(callback_key));
  }

  VALUE PropertyCallback::unwrapSetter(v8::Isolate* isolate, v8::Local<v8::Value> value) {
    v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(value);
    v8::Local<v8::String> callback_key = v8::String::NewFromUtf8(isolate, "rr::setter");

    return External::unwrap(holder->GetHiddenValue(callback_key));
  }

  VALUE PropertyCallback::unwrapQuery(v8::Isolate* isolate, v8::Local<v8::Value> value) {
    v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(value);
    v8::Local<v8::String> callback_key = v8::String::NewFromUtf8(isolate, "rr::query");

    return External::unwrap(holder->GetHiddenValue(callback_key));
  }

  VALUE PropertyCallback::unwrapDeleter(v8::Isolate* isolate, v8::Local<v8::Value> value) {
    v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(value);
    v8::Local<v8::String> callback_key = v8::String::NewFromUtf8(isolate, "rr::deleter");

    return External::unwrap(holder->GetHiddenValue(callback_key));
  }

  VALUE PropertyCallback::unwrapEnumerator(v8::Isolate* isolate, v8::Local<v8::Value> value) {
    v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(value);
    v8::Local<v8::String> callback_key = v8::String::NewFromUtf8(isolate, "rr::enumerator");

    return External::unwrap(holder->GetHiddenValue(callback_key));
  }

  void PropertyCallback::invokeGetter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapGetter(isolate, info.Data()));

    VALUE rb_property;
    if (property->IsSymbol()) {
      rb_property = Symbol(isolate, v8::Local<v8::Symbol>::Cast(property));
    } else {
      rb_property = String(isolate, property->ToString());
    }

    Unlocker unlock(info.GetIsolate());
    rb_funcall(code, rb_intern("call"), 2, rb_property, (VALUE)PropertyCallbackInfo::Value(info));
  }

  void PropertyCallback::invokeSetter(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapSetter(isolate, info.Data()));

    VALUE rb_property;
    if (property->IsSymbol()) {
      rb_property = Symbol(isolate, v8::Local<v8::Symbol>::Cast(property));
    } else {
      rb_property = String(isolate, property->ToString());
    }

    Unlocker unlock(info.GetIsolate());
    rb_funcall(
      code, rb_intern("call"), 3,
      rb_property,
      (VALUE)Value(isolate, value),
      (VALUE)PropertyCallbackInfo::Void(info)
    );
  }

  void PropertyCallback::invokeNamedPropertyGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapGetter(isolate, info.Data()));

    VALUE rb_property = String(isolate, property->ToString());

    Unlocker unlock(info.GetIsolate());
    rb_funcall(code, rb_intern("call"), 2, rb_property, (VALUE)PropertyCallbackInfo::Value(info));
  }

  void PropertyCallback::invokeNamedPropertySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapSetter(isolate, info.Data()));

    VALUE rb_property = String(isolate, property->ToString());

    Unlocker unlock(info.GetIsolate());
    rb_funcall(
      code, rb_intern("call"), 3,
      rb_property,
      (VALUE)Value(isolate, value),
      (VALUE)PropertyCallbackInfo::Value(info)
    );
  }

  void PropertyCallback::invokeNamedPropertyQuery(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Integer>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapQuery(isolate, info.Data()));

    VALUE rb_property = String(isolate, property->ToString());

    Unlocker unlock(info.GetIsolate());
    rb_funcall(code, rb_intern("call"), 2, rb_property, (VALUE)PropertyCallbackInfo::Integer(info));
  }

  void PropertyCallback::invokeNamedPropertyDeleter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Boolean>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapDeleter(isolate, info.Data()));

    VALUE rb_property = String(isolate, property->ToString());

    Unlocker unlock(info.GetIsolate());
    rb_funcall(code, rb_intern("call"), 2, rb_property, (VALUE)PropertyCallbackInfo::Boolean(info));
  }

  void PropertyCallback::invokeNamedPropertyEnumerator(const v8::PropertyCallbackInfo<v8::Array>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapEnumerator(isolate, info.Data()));

    Unlocker unlock(info.GetIsolate());
    rb_funcall(code, rb_intern("call"), 1, (VALUE)PropertyCallbackInfo::Array(info));
  }

  void PropertyCallback::invokeIndexedPropertyGetter(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapGetter(isolate, info.Data()));

    Unlocker unlock(info.GetIsolate());
    rb_funcall(code, rb_intern("call"), 2, UINT2NUM(index), (VALUE)PropertyCallbackInfo::Value(info));
  }

  void PropertyCallback::invokeIndexedPropertySetter(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapSetter(isolate, info.Data()));

    Unlocker unlock(info.GetIsolate());
    rb_funcall(
      code, rb_intern("call"), 3,
      UINT2NUM(index),
      (VALUE)Value(isolate, value),
      (VALUE)PropertyCallbackInfo::Value(info)
    );
  }

  void PropertyCallback::invokeIndexedPropertyQuery(uint32_t index, const v8::PropertyCallbackInfo<v8::Integer>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapQuery(isolate, info.Data()));

    Unlocker unlock(info.GetIsolate());
    rb_funcall(code, rb_intern("call"), 2, UINT2NUM(index), (VALUE)PropertyCallbackInfo::Integer(info));
  }

  void PropertyCallback::invokeIndexedPropertyDeleter(uint32_t index, const v8::PropertyCallbackInfo<v8::Boolean>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapDeleter(isolate, info.Data()));

    Unlocker unlock(info.GetIsolate());
    rb_funcall(code, rb_intern("call"), 2, UINT2NUM(index), (VALUE)PropertyCallbackInfo::Boolean(info));
  }

  void PropertyCallback::invokeIndexedPropertyEnumerator(const v8::PropertyCallbackInfo<v8::Array>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    VALUE code(unwrapEnumerator(isolate, info.Data()));

    Unlocker unlock(info.GetIsolate());
    rb_funcall(code, rb_intern("call"), 1, (VALUE)PropertyCallbackInfo::Array(info));
  }

}
