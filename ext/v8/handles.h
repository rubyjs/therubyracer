#ifndef RR_HANDLES
#define RR_HANDLES

namespace rr {

  class Handles {
  public:
    static void Init();
    static VALUE HandleScope(int argc, VALUE* argv, VALUE self);
  private:
    static VALUE SetupAndCall(Isolate isolate, int* state, VALUE code);
    static VALUE DoCall(VALUE code);
  };

}

#endif
