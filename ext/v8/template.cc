#include "rr.h"

namespace rr {
  void Template::Init() {
    ClassBuilder("Template");
    ObjectTemplate::Init();
    FunctionTemplate::Init();
  }

  void ObjectTemplate::Init() {
    ClassBuilder("ObjectTemplate", "Template").
      store(&Class);
  }

  void FunctionTemplate::Init() {
    ClassBuilder("FunctionTemplate", "Template").
      defineSingletonMethod("New", &New).
      defineMethod("GetFunction", &GetFunction).
      defineMethod("SetCallHandler", &SetCallHandler).
      defineMethod("InstanceTemplate", &InstanceTemplate).
      defineMethod("Inherit", &Inherit).
      defineMethod("PrototypeTemplate", &PrototypeTemplate).
      defineMethod("SetClassName", &SetClassName).
      defineMethod("SetHiddenPrototype", &SetHiddenPrototype).
      defineMethod("ReadOnlyPrototype", &ReadOnlyPrototype).
      defineMethod("HasInstance", &HasInstance).
      store(&Class);
  }

  VALUE FunctionTemplate::New(int argc, VALUE argv[], VALUE self) {
    VALUE code; VALUE data; VALUE signature;
    rb_scan_args(argc, argv, "03", &code, &data, &signature);
    if (RTEST(code)) {
      Invocation invocation(code, data);
      return FunctionTemplate(v8::FunctionTemplate::New(invocation, invocation, Signature(signature)));
    } else {
      return FunctionTemplate(v8::FunctionTemplate::New());
    }
  }

  VALUE FunctionTemplate::GetFunction(VALUE self) {
    return Function(FunctionTemplate(self)->GetFunction());
  }

  VALUE FunctionTemplate::SetCallHandler(int argc, VALUE argv[], VALUE self) {
    VALUE code; VALUE data;
    rb_scan_args(argc, argv, "11", &code, &data);
    Invocation invocation(code, data);
    Void(FunctionTemplate(self)->SetCallHandler(invocation, invocation));
  }

  VALUE FunctionTemplate::InstanceTemplate(VALUE self) {
    return ObjectTemplate(FunctionTemplate(self)->InstanceTemplate());
  }

  VALUE FunctionTemplate::Inherit(VALUE self, VALUE parent) {
    Void(FunctionTemplate(self)->Inherit(FunctionTemplate(parent)));
  }

  VALUE FunctionTemplate::PrototypeTemplate(VALUE self) {
    return ObjectTemplate(FunctionTemplate(self)->PrototypeTemplate());
  }

  VALUE FunctionTemplate::SetClassName(VALUE self, VALUE name) {
    Void(FunctionTemplate(self)->SetClassName(String(name)));
  }

  VALUE FunctionTemplate::SetHiddenPrototype(VALUE self, VALUE value) {
    Void(FunctionTemplate(self)->SetHiddenPrototype(Bool(value)));
  }

  VALUE FunctionTemplate::ReadOnlyPrototype(VALUE self) {
    Void(FunctionTemplate(self)->ReadOnlyPrototype());
  }

  VALUE FunctionTemplate::HasInstance(VALUE self, VALUE object) {
    return Bool(FunctionTemplate(self)->HasInstance(Value(object)));
  }
}