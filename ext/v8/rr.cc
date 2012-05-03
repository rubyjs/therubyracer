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

  ClassBuilder::ClassBuilder(const char* name, VALUE superclass) {
    this->value = defineClass(name, superclass);
  }

  ClassBuilder& ClassBuilder::defineMethod(const char* name, VALUE (*impl)(VALUE)) {
    rb_define_method(this->value, name, (VALUE (*)(...))impl, 0);
    return *this;
  }
  ClassBuilder& ClassBuilder::defineMethod(const char* name, VALUE (*impl)(VALUE, VALUE)) {
    rb_define_method(this->value, name, (VALUE (*)(...))impl, 1);
    return *this;
  }
  ClassBuilder& ClassBuilder::defineSingletonMethod(const char* name, VALUE (*impl)(VALUE)) {
    rb_define_singleton_method(this->value, name, (VALUE (*)(...))impl, 0);
    return *this;
  }
  ClassBuilder& ClassBuilder::defineSingletonMethod(const char* name, VALUE (*impl)(VALUE, VALUE)) {
    rb_define_singleton_method(this->value, name, (VALUE (*)(...))impl, 1);
    return *this;
  }
}