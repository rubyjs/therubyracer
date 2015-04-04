#ifndef RR_CONTEXT
#define RR_CONTEXT

namespace rr {

  class Context : public Ref<v8::Context> {
  public:
    static void Init();

    static VALUE New(int argc, VALUE argv[], VALUE self);
    static VALUE Dispose(VALUE self);

    static VALUE Enter(VALUE self);
    static VALUE Exit(VALUE self);

    // TODO
    // static VALUE Global(VALUE self);
    // static VALUE DetachGlobal(VALUE self);
    // static VALUE ReattachGlobal(VALUE self, VALUE global);
    // static VALUE GetEntered(VALUE self);
    // static VALUE GetCurrent(VALUE self);
    // static VALUE GetCalling(VALUE self);
    // static VALUE SetSecurityToken(VALUE self, VALUE token);
    // static VALUE UseDefaultSecurityToken(VALUE self);
    // static VALUE GetSecurityToken(VALUE self);
    // static VALUE HasOutOfMemoryException(VALUE self);
    // static VALUE InContext(VALUE self);
    // static VALUE SetEmbedderData(VALUE self, VALUE index, VALUE data);
    // static VALUE GetEmbedderData(VALUE self, VALUE index);
    // static VALUE AllowCodeGenerationFromStrings(VALUE self, VALUE allow);
    // static VALUE IsCodeGenerationFromStringsAllowed(VALUE self);

    inline Context(VALUE value) : Ref<v8::Context>(value) {}
    inline Context(v8::Handle<v8::Context> ctx) : Ref<v8::Context>(ctx->GetIsolate(), ctx) {}
  };

}

#endif
