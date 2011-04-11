#ifndef _RR_V8_HANDLE_
#define _RR_V8_HANDLE_ 

#include <v8.h>
#include "ruby.h"

struct v8_handle  {
  v8_handle(v8::Handle<void> object);
  virtual ~v8_handle();
  v8::Persistent<void> handle;
};

void rr_init_handle();

template <class T> v8::Persistent<T>& rr_v8_handle(VALUE value) {
  v8_handle* handle = 0;
  Data_Get_Struct(value, struct v8_handle, handle);
  return (v8::Persistent<T>&)handle->handle;
}
VALUE rr_v8_handle_new(VALUE rbclass, v8::Handle<void> handle);
VALUE rr_v8_handle_class();

#endif
