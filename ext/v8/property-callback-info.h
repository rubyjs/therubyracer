// -*- mode: c++ -*-
#ifndef RR_PROPERTY_CALLBACK_INFO_H
#define RR_PROPERTY_CALLBACK_INFO_H

namespace rr {

  class PropertyCallbackInfo {
  public:

    template <class T>
    class Base : public Wrapper<v8::PropertyCallbackInfo<T>> {
    public:

      inline Base(v8::PropertyCallbackInfo<T> info) :
        Wrapper<v8::PropertyCallbackInfo<T>>(info) {}

      inline Base(VALUE self) : Wrapper<v8::PropertyCallbackInfo<T>>(self) {}

      static VALUE This(VALUE self) {
        Base<T> info(self);
        Locker lock(info->GetIsolate());
        return Object(info->GetIsolate(), info->This());
      }

      static VALUE Data(VALUE self) {
        Base<T> info(self);
        Isolate isolate(info->GetIsolate());
        Locker lock(isolate);

        return PropertyCallback::unwrapData(isolate, info->Data()) ;
      }

      static VALUE GetIsolate(VALUE self) {
        Base<T> info(self);
        return Isolate(info->GetIsolate());
      }

    };

    class Value : public Base<v8::Value> {
    public:

      inline Value(v8::PropertyCallbackInfo<v8::Value> info) :
        Base<v8::Value>(info) {}

      inline Value(VALUE self) : Base<v8::Value>(self) {}

      static VALUE GetReturnValue(VALUE self) {
        Value info(self);
        Locker lock(info->GetIsolate());
        return ReturnValue::Value(info->GetReturnValue());
      }

      static inline void Init() {
        ClassBuilder("Value", PropertyCallbackInfo::Class, PropertyCallbackInfo::Class).
          defineMethod("This", &This).
          defineMethod("Data", &Data).
          defineMethod("GetIsolate", &GetIsolate).
          defineMethod("GetReturnValue", &GetReturnValue).
          store(&Class);
      }

    };

    class Void : public Base<void> {
    public:

      inline Void(v8::PropertyCallbackInfo<void> info) :
        Base<void>(info) {}

      inline Void(VALUE self) : Base<void>(self) {}

      static VALUE GetReturnValue(VALUE self) {
        Value info(self);
        Locker lock(info->GetIsolate());
        return ReturnValue::Void(info->GetReturnValue());
      }

      static inline void Init() {
        ClassBuilder("Void", PropertyCallbackInfo::Class, PropertyCallbackInfo::Class).
          defineMethod("This", &This).
          defineMethod("Data", &Data).
          defineMethod("GetIsolate", &GetIsolate).
          defineMethod("GetReturnValue", &GetReturnValue).
          store(&Class);
      }

    };

    static VALUE Class;

    static void Init() {
      ClassBuilder("PropertyCallbackInfo").
        store(&Class);

      Value::Init();
      Void::Init();
    }

  };

}

#endif /* RR_PROPERTY_CALLBACK_INFO_H */
