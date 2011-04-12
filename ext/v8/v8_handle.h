#ifndef _RR_V8_HANDLE_
#define _RR_V8_HANDLE_ 

#include <v8.h>
#include "ruby.h"

struct v8_handle  {
  v8_handle(v8::Handle<void> object);
  virtual ~v8_handle();
  VALUE get_internal(const char* name);
  void set_internal(const char* name, VALUE value);

  v8::Persistent<void> handle;
  bool dead;
  VALUE references;
};

void rr_init_handle();

v8_handle* rr_v8_handle_raw(VALUE value);

template <class T> v8::Persistent<T>& rr_v8_handle(VALUE value) {
  return (v8::Persistent<T>&)(rr_v8_handle_raw(value)->handle);
}
VALUE rr_v8_handle_new(VALUE rbclass, v8::Handle<void> handle);
VALUE rr_v8_handle_class();

void rr_v8_handle_set_internal(VALUE handle, const char* name, VALUE value);
VALUE rr_v8_handle_get_internal(VALUE handle, const char* name);


#endif
