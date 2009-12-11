#include <v8_ref.h>
#include "stdio.h"
using namespace v8;


v8_ref::v8_ref(Handle<void> object, VALUE ref) : handle(Persistent<void>::New(object)), references(ref) {
  printf("Allocating v8 reference\n");
}

v8_ref::~v8_ref() {  
  printf("Disposing v8 reference\n");
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

