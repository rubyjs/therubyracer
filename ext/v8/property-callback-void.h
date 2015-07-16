// -*- mode: c++ -*-
#ifndef RR_PROPERTY_CALLBACK_VOID_H
#define RR_PROPERTY_CALLBACK_VOID_H

namespace rr {

  typedef Wrapper<v8::PropertyCallbackInfo<void>> PropertyCallbackInfoVoidWrapper;

  class PropertyCallbackInfoVoid : public PropertyCallbackInfoVoidWrapper {
  public:

    inline PropertyCallbackInfoVoid(v8::PropertyCallbackInfo<void> info) :
      PropertyCallbackInfoVoidWrapper(info) {}

    inline PropertyCallbackInfoVoid(VALUE self) : PropertyCallbackInfoVoidWrapper(self) {}

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::AccessorNameSetterCallback` API.
     */
    static void invokeSetter(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
      v8::Isolate* isolate = info.GetIsolate();

      v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(info.Data());
      v8::Local<v8::String> callback_key = v8::String::NewFromUtf8(isolate, "rr::setter");

      VALUE code(External::unwrap(holder->GetHiddenValue(callback_key)));

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
        (VALUE)Value::handleToRubyObject(isolate, value),
        (VALUE)PropertyCallbackInfoVoid(info)
      );
    }

    static VALUE This(VALUE self) {
      PropertyCallbackInfoVoid info(self);
      Locker lock(info->GetIsolate());
      return Object(info->GetIsolate(), info->This());
    }

    static VALUE Data(VALUE self) {
      PropertyCallbackInfoVoid info(self);
      Isolate isolate(info->GetIsolate());
      Locker lock(isolate);

      v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(info->Data());
      v8::Local<v8::String> data_key = v8::String::NewFromUtf8(isolate, "rr::data");
      v8::Local<v8::Value> data(holder->GetHiddenValue(data_key));

      return Value::handleToRubyObject(info->GetIsolate(), data);
    }

    static VALUE GetIsolate(VALUE self) {
      PropertyCallbackInfoVoid info(self);
      return Isolate(info->GetIsolate());
    }

    static inline void Init() {
      ClassBuilder("PropertyCallbackVoidInfo").
        defineMethod("This", &This).
        defineMethod("Data", &Data).
        defineMethod("GetIsolate", &GetIsolate).
        store(&Class);
    }

    v8::Local<v8::Value> data;
  };
}

#endif /* RR_PROPERTY_CALLBACK_VOID_H */
