#ifndef _RUBY_V8_CXT_
#define _RUBY_V8_CXT_ 

#include "ruby.h"
#include "v8.h"
#include "v8_ref.h"

class v8_cxt : v8_ref<v8::Context> {
public:
  v8_cxt();
  ~v8_cxt();
};

//memory management
VALUE v8_cxt_allocate(VALUE clazz);
#endif