#include "rr.h"

namespace rr {

  v8::Local<v8::Value> PropertyCallback::wrapData(v8::Isolate* isolate, VALUE r_getter, VALUE r_setter, VALUE r_data) {
    v8::Local<v8::Object> holder = v8::Object::New(isolate);

    v8::Local<v8::String> getter_key = v8::String::NewFromUtf8(isolate, "rr::getter");
    v8::Local<v8::String> setter_key = v8::String::NewFromUtf8(isolate, "rr::setter");
    v8::Local<v8::String> data_key = v8::String::NewFromUtf8(isolate, "rr::data");

    holder->SetHiddenValue(getter_key, External::wrap(isolate, r_getter));
    holder->SetHiddenValue(setter_key, External::wrap(isolate, r_setter));
    holder->SetHiddenValue(data_key, rr::Value(r_data));

    return holder;
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

}
