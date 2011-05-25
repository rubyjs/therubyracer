#ifndef _RR_V8_WEAKREF_
#define _RR_V8_WEAKREF_ 

#include <v8.h>
#include "ruby.h"
#include "rr.h"

struct v8_weakref  {
  v8_weakref(VALUE object);
  VALUE get();
  void set(VALUE object);
  void retain();
  void release();

  VALUE object_id;
  bool v8_active;
  bool rb_active;
  v8::Persistent<v8::External> external;
};

void  v8_weakref_dispose(v8::Persistent<v8::Value> value, void* weakref);
VALUE v8_weakref_finalize(VALUE self, VALUE object_id);
VALUE v8_weakref_objectspace();
VALUE v8_weakref_nil(VALUE nil, VALUE exception);
VALUE v8_weakref_id2ref(VALUE id);

// extern "C" VALUE rb_proc_new(VALUE (*)(ANYARGS/* VALUE yieldarg[, VALUE procarg] */), VALUE);

#endif
