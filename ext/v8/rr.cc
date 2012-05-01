#include "rr.h"

namespace rr {
  VALUE defineClass(const char *name, VALUE superclass) {
    VALUE V8 = rb_define_module("V8");
    VALUE V8_C = rb_define_module_under(V8, "C");
    VALUE klass = rb_define_class_under(V8_C, name, superclass);
    rb_funcall(klass, rb_intern("private_class_method"), 1, rb_str_new2("new"));
    return klass;
  }

  VALUE defineModule(const char *name) {
    VALUE V8 = rb_define_module("V8");
    VALUE V8_C = rb_define_module_under(V8, "C");
    return rb_define_module_under(V8_C, name);
  }  
}