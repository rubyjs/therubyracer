#include "rr.h"

namespace rr {
  VALUE queue;

  namespace gc {
    VALUE dequeue(VALUE q) {
      return rb_funcall(q, rb_intern("pop"), 1, Qtrue);
    }
  }
  void GC::Push(VALUE phantom) {
    rb_funcall(queue, rb_intern("<<"), 1, phantom);
  }
  void GC::Pop(v8::GCType type, v8::GCCallbackFlags flags) {
    VALUE next = rb_protect(&gc::dequeue, queue, NULL);
    rb_set_errinfo(Qnil);
    if (!NIL_P(next)) {
      Phantom(next).destroy();
    }
  }
  void GC::Init() {
    rb_gc_register_address(&queue);
    queue = rb_eval_string("require 'thread'; Queue.new");
  }
}