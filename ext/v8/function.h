// -*- mode: c++ -*-
#ifndef RR_FUNCTION
#define RR_FUNCTION

namespace rr {

  class Function : public Ref<v8::Function> {
  public:
    static void Init();

    static VALUE New(int argc, VALUE argv[], VALUE self);
    static VALUE NewInstance(int argc, VALUE argv[], VALUE self);
    static VALUE Call(VALUE self, VALUE receiver, VALUE arguments);
    static VALUE SetName(VALUE self, VALUE name);
    static VALUE GetName(VALUE self);
    static VALUE GetInferredName(VALUE self);
    static VALUE GetDisplayName(VALUE self);
    static VALUE GetScriptLineNumber(VALUE self);
    static VALUE GetScriptColumnNumber(VALUE self);
    static VALUE IsBuiltin(VALUE self);
    static VALUE ScriptId(VALUE self);
    static VALUE GetBoundFunction(VALUE self);
    static VALUE GetScriptOrigin(VALUE self);

    inline Function(VALUE value) : Ref<v8::Function>(value) {}
    inline Function(v8::Isolate* isolate, v8::Handle<v8::Function> function) : Ref<v8::Function>(isolate, function) {}
    typedef MaybeLocal<Function, v8::Function> Maybe;
  };
}

#endif
