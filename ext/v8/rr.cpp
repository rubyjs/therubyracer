#include "rr.h"

VALUE rr_define_class(const char *name) {
  VALUE V8 = rb_define_module("V8");
  VALUE V8_C = rb_define_module_under(V8, "C");
  return rb_define_class_under(V8_C, name, rb_cObject);
}