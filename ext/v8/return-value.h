// -*- mode: c++ -*-
#ifndef RR_RETURN_VALUE_H
#define RR_RETURN_VALUE_H

namespace rr {
  typedef Wrapper<v8::ReturnValue<v8::Value>> ReturnValueWrapper;

  class ReturnValue : public ReturnValueWrapper {
  public:
    ReturnValue(v8::ReturnValue<v8::Value> value) : ReturnValueWrapper(value) {}
    ReturnValue(VALUE self) : ReturnValueWrapper(self) {}

    static VALUE Set(VALUE self, VALUE handle) {
      ReturnValue ret(self);
      Locker lock(ret->GetIsolate());
      v8::Local<v8::Value> value((Value(handle)));
      ret->Set(value);
      return Qnil;
    }

    static VALUE Set_bool(VALUE self, VALUE value) {
      ReturnValue ret(self);
      Locker lock(ret->GetIsolate());
      ret->Set((bool)Bool(value));
      return Qnil;
    }

    static VALUE Set_double(VALUE self, VALUE value) {
      ReturnValue ret(self);
      Locker lock(ret->GetIsolate());
      ret->Set(NUM2DBL(value));
      return Qnil;
    }

    static VALUE Set_int32_t(VALUE self, VALUE i) {
      ReturnValue ret(self);
      Locker lock(ret->GetIsolate());
      ret->Set(NUM2INT(i));
      return Qnil;
    }

    static VALUE Set_uint32_t(VALUE self, VALUE i) {
      ReturnValue ret(self);
      Locker lock(ret->GetIsolate());
      ret->Set(NUM2UINT(i));
      return Qnil;
    }

    static VALUE SetNull(VALUE self) {
      ReturnValue ret(self);
      Locker lock(ret->GetIsolate());
      ret->SetNull();
      return Qnil;
    }

    static VALUE SetUndefined(VALUE self) {
      ReturnValue ret(self);
      Locker lock(ret->GetIsolate());
      ret->SetUndefined();
      return Qnil;
    }

    static VALUE SetEmptyString(VALUE self) {
      ReturnValue ret(self);
      Locker lock(ret->GetIsolate());
      ret->SetEmptyString();
      return Qnil;
    }

    static VALUE GetIsolate(VALUE self) {
      ReturnValue ret(self);
      return Isolate(ret->GetIsolate());
    }

    static inline void Init() {
      ClassBuilder("ReturnValue").
        defineMethod("Set", &Set).
        defineMethod("Set_bool", &Set_bool).
        defineMethod("Set_double", &Set_double).
        defineMethod("Set_int32_t", &Set_int32_t).
        defineMethod("Set_uint32_t", &Set_uint32_t).
        defineMethod("SetNull", &SetNull).
        defineMethod("SetUndefined", &SetUndefined).
        defineMethod("SetEmptyString", &SetEmptyString).
        defineMethod("GetIsolate", &GetIsolate).
        store(&Class);
    }
  };
}
#endif /* RR_RETURN_VALUE_H */
