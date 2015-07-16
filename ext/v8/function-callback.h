// -*- mode: c++ -*-
#ifndef RR_FUNCTION_CALLBACK_H
#define RR_FUNCTION_CALLBACK_H

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

  typedef Wrapper<v8::FunctionCallbackInfo<v8::Value>> FunctionCallbackInfoWrapper;

  class FunctionCallbackInfo : public FunctionCallbackInfoWrapper {
  public:

    inline FunctionCallbackInfo(v8::FunctionCallbackInfo<v8::Value> info, v8::Local<v8::Value> data_) :
      FunctionCallbackInfoWrapper(info), data(data_) {}

    inline FunctionCallbackInfo(VALUE self) : FunctionCallbackInfoWrapper(self) {}

    inline v8::Local<v8::Value> operator [](int i) {
      return this->container->content[i];
    }

    static v8::Local<v8::Value> wrapData(v8::Isolate* isolate, VALUE r_callback, VALUE r_data) {
      v8::Local<v8::Object> holder = v8::Object::New(isolate);
      v8::Local<v8::String> callback_key = v8::String::NewFromUtf8(isolate, "rr::callback");
      v8::Local<v8::String> data_key = v8::String::NewFromUtf8(isolate, "rr::data");
      holder->SetHiddenValue(callback_key, External::wrap(isolate, r_callback));
      holder->SetHiddenValue(data_key, Value(r_data));
      return holder;
    }

    static void invoke(const v8::FunctionCallbackInfo<v8::Value>& info) {
      v8::Isolate* isolate = info.GetIsolate();
      v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast<v8::Value>(info.Data());
      v8::Local<v8::String> data_key = v8::String::NewFromUtf8(isolate, "rr::data");
      v8::Local<v8::String> callback_key = v8::String::NewFromUtf8(isolate, "rr::callback");
      v8::Local<v8::Value> data(holder->GetHiddenValue(data_key));

      VALUE code(External::unwrap(holder->GetHiddenValue(callback_key)));
      Unlocker unlock(info.GetIsolate());
      rb_funcall(code, rb_intern("call"), 1, (VALUE)FunctionCallbackInfo(info, data));
    }

    static VALUE Length(VALUE self) {
      FunctionCallbackInfo info(self);
      Locker lock(info->GetIsolate());
      return INT2FIX(info->Length());
    }

    static VALUE at(VALUE self, VALUE i) {
      FunctionCallbackInfo info(self);
      Locker lock(info->GetIsolate());
      return Value::handleToRubyObject(info->GetIsolate(), info[NUM2INT(i)]);
    }

    static VALUE Callee(VALUE self) {
      FunctionCallbackInfo info(self);
      Locker lock(info->GetIsolate());
      return Function(info->GetIsolate(), info->Callee());
    }

    static VALUE This(VALUE self) {
      FunctionCallbackInfo info(self);
      Locker lock(info->GetIsolate());
      return Object(info->GetIsolate(), info->This());
    }

    static VALUE IsConstructCall(VALUE self) {
      FunctionCallbackInfo info(self);
      Locker lock(info->GetIsolate());
      return Bool(info->IsConstructCall());
    }

    static VALUE Data(VALUE self) {
      FunctionCallbackInfo info(self);
      Locker lock(info->GetIsolate());
      return Value::handleToRubyObject(info->GetIsolate(), info.data);
    }

    static VALUE GetIsolate(VALUE self) {
      FunctionCallbackInfo info(self);
      return Isolate(info->GetIsolate());
    }

    static VALUE GetReturnValue(VALUE self) {
      FunctionCallbackInfo info(self);
      Locker lock(info->GetIsolate());
      return ReturnValue(info->GetReturnValue());
    }

    static inline void Init() {
      ClassBuilder("FunctionCallbackInfo").
        defineMethod("Length", &Length).
        defineMethod("[]", &at).
        defineMethod("Callee", &Callee).
        defineMethod("This", &This).
        defineMethod("IsConstructCall", &IsConstructCall).
        defineMethod("Data", &Data).
        defineMethod("GetIsolate", &GetIsolate).
        defineMethod("GetReturnValue", &GetReturnValue).
        store(&Class);
    }

    v8::Local<v8::Value> data;
  };
}

#endif /* RR_FUNCTION_CALLBACK_H */
