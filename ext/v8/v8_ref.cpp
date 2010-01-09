#include <v8_ref.h>
#include "stdio.h"
using namespace v8;


v8_ref::v8_ref(Handle<void> object, VALUE ref) : handle(Persistent<void>::New(object)), references(ref) {

}

v8_ref::~v8_ref() {  
  handle.Dispose();
}

void v8_ref_mark(v8_ref* ref) {
  if (ref->references != 0) {
    rb_gc_mark(ref->references);
  }
}

void v8_ref_free(v8_ref* ref) {
  delete ref;
}

VALUE V8_Ref_Create(VALUE ruby_class, v8::Handle<void> handle, VALUE ref) {
  return Data_Wrap_Struct(ruby_class, v8_ref_mark, v8_ref_free, new v8_ref(handle, ref));
}
