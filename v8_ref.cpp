#include <v8_ref.h>
#include "stdio.h"
using namespace v8;

template <class T> v8_ref<T>::v8_ref(Handle<T> object) : handle(*object) {
  printf("Allocated v8 reference\n");
}

template <class T> v8_ref<T>::~v8_ref() {  
  printf("Disposing of v8 reference\n");
  handle.Dispose();
}

void v8_ref_mark(v8_ref_data* ref) {
  
}

void v8_ref_free(v8_ref_data* ref) {
  delete ref;
}

