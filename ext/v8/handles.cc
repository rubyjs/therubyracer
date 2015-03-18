#include "rr.h"

namespace rr {

  void Handles::Init() {
    VALUE v8 = rb_define_module("V8");
    VALUE c = rb_define_module_under(v8, "C");
    rb_define_singleton_method(c, "HandleScope", (VALUE (*)(...))&HandleScope, -1);
  }

  VALUE Handles::HandleScope(int argc, VALUE* argv, VALUE self) {
    if (!rb_block_given_p()) {
      return Qnil;
    }

    VALUE isolate, block;
    rb_scan_args(argc, argv, "10&", &isolate, &block);

    int state = 0;
    VALUE result = SetupAndCall(Isolate(isolate), &state, block);
    if (state != 0) {
      rb_jump_tag(state);
    }
    return result;
  }

  VALUE Handles::SetupAndCall(Isolate isolate, int* state, VALUE block) {
    v8::HandleScope handle_scope(isolate);
    return rb_protect(&DoCall, block, state);
  }

  VALUE Handles::DoCall(VALUE block) {
    return rb_funcall(block, rb_intern("call"), 0);
  }

}
