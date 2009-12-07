#include <v8_ref.h>
#include "stdio.h"
using namespace v8;


v8_ref::v8_ref(Handle<void> object) : handle(Persistent<void>::New(object)) {
  printf("Allocating v8 reference\n");
}

v8_ref::~v8_ref() {  
  printf("Disposing of v8 reference\n");
  if (!handle.IsEmpty()) {
    handle.Dispose();
  }
}

void v8_ref_mark(v8_ref* ref) {
  
}

void v8_ref_free(v8_ref* ref) {
  delete ref;
}

