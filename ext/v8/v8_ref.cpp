#include <v8_ref.h>
#include "stdio.h"
using namespace v8;

v8_ref::v8_ref(Handle<void> object) : handle(Persistent<void>::New(object)) {
  this->references = rb_hash_new();
}

v8_ref::~v8_ref() {  
  handle.Dispose();
}

void v8_ref::set(const char *name, VALUE ref) {
  if (ref != 0 && RTEST(ref)) {
    rb_hash_aset(this->references, rb_str_new2(name), ref);    
  }
}

namespace {
  void gc_mark(v8_ref* ref) {
    rb_gc_mark(ref->references);
  }

  void gc_free(v8_ref* ref) {
    delete ref;
  }
}

VALUE rr_v8_ref_create(VALUE rbclass, v8::Handle<void> handle) {
  return Data_Wrap_Struct(rbclass, gc_mark, gc_free, new v8_ref(handle));
}

void rr_v8_ref_setref(VALUE handle, const char *name, VALUE newref) {
  v8_ref *ref = 0;
  Data_Get_Struct(handle, struct v8_ref, ref);
  ref->set(name, newref);
}