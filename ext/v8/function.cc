#include "rr.h"

namespace rr {

  void Function::Init() {
    ClassBuilder("Function", Object::Class).
      defineSingletonMethod("New", &New).
      defineMethod("NewInstance", &NewInstance).
      defineMethod("Call", &Call).
      defineMethod("SetName", &SetName).
      defineMethod("GetName", &GetName).
      defineMethod("GetInferredName", &GetInferredName).
      defineMethod("GetScriptLineNumber", &GetScriptLineNumber).
      defineMethod("GetScriptColumnNumber", &GetScriptColumnNumber).
      defineMethod("IsBuiltin", &IsBuiltin).
      defineMethod("ScriptId", &ScriptId).
      defineMethod("GetBoundFunction", &GetBoundFunction).
      defineMethod("GetScriptOrigin", &GetScriptOrigin).

      store(&Class);
  }

  VALUE Function::New(int argc, VALUE argv[], VALUE self) {
    VALUE r_isolate, r_callback, r_data, r_length;
    rb_scan_args(argc, argv, "22", &r_isolate, &r_callback, &r_data, &r_length);
    Isolate isolate(r_isolate);
    Locker lock(isolate);

    v8::Local<v8::Value> data(FunctionCallbackInfo::wrapData(isolate, r_callback, r_data));

    int length = RTEST(r_length) ? NUM2INT(r_length) : 0;

    v8::FunctionCallback callback = &FunctionCallbackInfo::invoke;

    return Function(isolate, v8::Function::New(isolate,  callback, data, length));
  }

  VALUE Function::NewInstance(int argc, VALUE argv[], VALUE self) {
    VALUE args;
    rb_scan_args(argc, argv, "01", &args);

    Function function(self);
    v8::Isolate* isolate = function.getIsolate();
    Locker lock(isolate);

    if (RTEST(args)) {
      std::vector< v8::Handle<v8::Value> > vector(Value::convertRubyArray(isolate, args));
      return Object(isolate, function->NewInstance(RARRAY_LENINT(args), &vector[0]));
    } else {
      return Object(isolate, function->NewInstance());
    }
  }

  VALUE Function::Call(VALUE self, VALUE receiver, VALUE argv) {
    Function function(self);
    v8::Isolate* isolate = function.getIsolate();
    Locker lock(isolate);

    std::vector< v8::Handle<v8::Value> > vector(Value::convertRubyArray(isolate, argv));

    return Value::handleToRubyObject(isolate, function->Call(Value(receiver), RARRAY_LENINT(argv), &vector[0]));
  }

  VALUE Function::SetName(VALUE self, VALUE name) {
    Function function(self);
    Locker lock(function.getIsolate());

    function->SetName(String(name));

    return Qnil;
  }

  VALUE Function::GetName(VALUE self) {
    Function function(self);
    Locker lock(function.getIsolate());

    return Value::handleToRubyObject(function.getIsolate(), function->GetName());
  }

  VALUE Function::GetInferredName(VALUE self) {
    Function function(self);
    Locker lock(function.getIsolate());

    return Value::handleToRubyObject(function.getIsolate(), function->GetInferredName());
  }

  VALUE Function::GetScriptLineNumber(VALUE self) {
    Function function(self);
    Locker lock(function.getIsolate());

    return INT2FIX(function->GetScriptLineNumber());
  }

  VALUE Function::GetScriptColumnNumber(VALUE self) {
    Function function(self);
    Locker lock(function.getIsolate());

    return INT2FIX(function->GetScriptColumnNumber());
  }

  VALUE Function::IsBuiltin(VALUE self) {
    Function function(self);
    Locker lock(function);

    return Bool(function->IsBuiltin());
  }

  VALUE Function::ScriptId(VALUE self) {
    Function function(self);
    Locker lock(function.getIsolate());

    return INT2FIX(function->ScriptId());
  }

  VALUE Function::GetBoundFunction(VALUE self) {
    Function function(self);
    Locker lock(function);

    return Value(function.getIsolate(), function->GetBoundFunction());
  }

  VALUE Function::GetScriptOrigin(VALUE self) {
    Function function(self);
    Locker lock(function);

    return ScriptOrigin(function, function->GetScriptOrigin());
  }

}
