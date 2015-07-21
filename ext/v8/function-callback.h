// -*- mode: c++ -*-
#ifndef RR_FUNCTION_CALLBACK_H
#define RR_FUNCTION_CALLBACK_H

namespace rr {

  typedef Wrapper<v8::FunctionCallbackInfo<v8::Value>> FunctionCallbackInfoWrapper;

  class FunctionCallbackInfo : public FunctionCallbackInfoWrapper {
  public:

    inline FunctionCallbackInfo(v8::FunctionCallbackInfo<v8::Value> info, v8::Local<v8::Value> data_) :
      FunctionCallbackInfoWrapper(info), data(data_) {}

    inline FunctionCallbackInfo(VALUE self) : FunctionCallbackInfoWrapper(self) {}

    inline v8::Local<v8::Value> operator [](int i) {
      return this->container->content[i];
    }

    /**
     * Package up the callback data for this function so that it can
     * invoke a Ruby callable.
     *
     * Each `v8::Function` can have one `v8::Value` associated with it
     * that is passed to its `v8::FunctionCallback`. To support this
     * same API from ruby, we take the `Value` passed into the
     * Function constructor *and* the callback and store them *both*
     * in a single `v8::Object` which we use for the C++ level
     * callback data.
     */
    static v8::Local<v8::Value> wrapData(v8::Isolate* isolate, VALUE r_callback, VALUE r_data) {
      v8::Local<v8::Object> holder = v8::Object::New(isolate);
      v8::Local<v8::String> callback_key = v8::String::NewFromUtf8(isolate, "rr::callback");
      v8::Local<v8::String> data_key = v8::String::NewFromUtf8(isolate, "rr::data");
      holder->SetHiddenValue(callback_key, External::wrap(isolate, r_callback));
      holder->SetHiddenValue(data_key, Value(r_data));
      return holder;
    }

    /**
     * Call the Ruby code associated with this callback.
     *
     * Unpack the Ruby code, and the callback data from the C++
     * callback data, and then invoke that code.
     *
     * Note: This function implements the `v8::FunctionCallback` API.
     */
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
      return Value(info->GetIsolate(), info[NUM2INT(i)]);
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
      return Value(info->GetIsolate(), info.data);
    }

    static VALUE GetIsolate(VALUE self) {
      FunctionCallbackInfo info(self);
      return Isolate(info->GetIsolate());
    }

    static VALUE GetReturnValue(VALUE self) {
      FunctionCallbackInfo info(self);
      Locker lock(info->GetIsolate());
      return ReturnValue::Value(info->GetReturnValue());
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
