#ifndef RR_SCRIPT
#define RR_SCRIPT

namespace rr {

  class Script : public Ref<v8::Script> {
  public:
    static void Init();
    static VALUE Compile(int argc, VALUE argv[], VALUE self);
    static VALUE Run(VALUE self, VALUE context);
    // static VALUE RunWithTimeout(VALUE self, VALUE timeout);

    inline Script(VALUE value) : Ref<v8::Script>(value) {}
    inline Script(v8::Isolate* isolate, v8::Handle<v8::Script> script) : Ref<v8::Script>(isolate, script) {}
  };

}

#endif
