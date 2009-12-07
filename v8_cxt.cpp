
#include "v8_cxt.h"
#include "ruby.h"
#include "stdio.h"

using namespace v8;

v8_cxt::v8_cxt() : v8_ref<Context>(Context::New()) {
  printf("Allocate Native V8 Context\n");
}

VALUE v8_cxt_allocate(VALUE clazz) {
  printf("This is the big leagues chew!\n");
  v8_cxt* cxt = new v8_cxt();
  return Data_Wrap_Struct(clazz, 0, 0, cxt);
}


