#ifndef THE_RUBY_RACER
#define THE_RUBY_RACER

#include <stdio.h>
#include <v8.h>
#include <ruby.h>

#define RR_DEFINE_METHOD(klass, name, impl, argc) rb_define_method(klass, name, (VALUE(*)(...))impl, argc)

namespace rr {

class Object {
public:
  Object(VALUE value);
  inline operator VALUE() {return value;};
private:
  VALUE value;
};

class Class {
public:
  Class(const char* name);
  Class defineSingletonMethod(const char* name, VALUE(*impl)(...), int argc);
  Class defineMethod(const char* name, VALUE(*impl)(...), int argc);
};

class Context {
public:
  static void Initialize();
};

}

#endif
