#include <v8_ref.h>
#include "stdio.h"
using namespace v8;

v8_ref::v8_ref(Handle<void> object, VALUE ref) : handle(Persistent<void>::New(object)) {
  references = rb_hash_new();
  this->add(ref);
}

void v8_ref::add(VALUE ref) {
  if (ref != 0 && RTEST(ref)) {
    rb_hash_aset(references, ref, Qtrue);
  }
}

v8_ref::~v8_ref() {  
  handle.Dispose();
}

namespace {
  //the v8_ref wraps a v8 handle so that ruby can hold a reference to it.
  void gc_mark(v8_ref* ref) {
    rb_gc_mark(ref->references);
  }

  void gc_free(v8_ref* ref) {
    delete ref;
  }
}

void rr_ref_addref(VALUE data, VALUE newref) {
  v8_ref * ref = 0;
  Data_Get_Struct(data, struct v8_ref, ref);
  ref->add(newref);
}

VALUE V8_Ref_Create(VALUE ruby_class, v8::Handle<void> handle, VALUE ref) {
  return Data_Wrap_Struct(ruby_class, gc_mark, gc_free, new v8_ref(handle, ref));
}
