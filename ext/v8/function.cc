#include "rr.h"

namespace rr {
  void Function::Init() {
    ClassBuilder("Function", Object::Class).
      defineMethod("NewInstance", &NewInstance).
      defineMethod("Call", &Call).
      defineMethod("SetName", &SetName).
      defineMethod("GetName", &GetName).
      defineMethod("GetInferredName", &GetInferredName).
      defineMethod("GetScriptLineNumber", &GetScriptLineNumber).
      defineMethod("GetScriptColumnNumber", &GetScriptColumnNumber).
      defineMethod("GetScriptId", &GetScriptId).
      defineMethod("GetScriptOrigin", &GetScriptOrigin).
      store(&Class);
  }

  VALUE Function::NewInstance(int i, VALUE v[], VALUE self) {
    if (i == 0) {
      return Object(Function(self)->NewInstance());
    } else {
      VALUE argc; VALUE argv;
      rb_scan_args(i,v,"2", &argc, &argv);
      std::vector< v8::Handle<v8::Value> > arguments(NUM2INT(argc));
      return Object(Function(self)->NewInstance(NUM2INT(argc), Value::array(argv, arguments)));
    }
  }
  VALUE Function::Call(VALUE self, VALUE receiver, VALUE argc, VALUE argv) {
    std::vector< v8::Handle<v8::Value> > arguments(NUM2INT(argc));
    return Value(Function(self)->Call(Object(receiver), NUM2INT(argc), Value::array(argv, arguments)));
  }

  VALUE Function::SetName(VALUE self, VALUE name) {
    Void(Function(self)->SetName(String(name)));
  }

  VALUE Function::GetName(VALUE self) {
    return Value(Function(self)->GetName());
  }

  VALUE Function::GetInferredName(VALUE self) {
    return Value(Function(self)->GetInferredName());
  }

  VALUE Function::GetScriptLineNumber(VALUE self) {
    return INT2FIX(Function(self)->GetScriptLineNumber());
  }

  VALUE Function::GetScriptColumnNumber(VALUE self) {
    return INT2FIX(Function(self)->GetScriptColumnNumber());
  }

  VALUE Function::GetScriptId(VALUE self) {
    return Value(Function(self)->GetScriptId());
  }

  VALUE Function::GetScriptOrigin(VALUE self) {
    return not_implemented("GetScriptOrigin");
  }
}