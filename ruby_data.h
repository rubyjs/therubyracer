#ifndef __ruby_data_h__
#define __ruby_data_h__

#include <ruby.h>

template<class T, class R> class RubyDataSource {

  T dest;

 public:
  RubyDataSource() {}
  ~RubyDataSource() {}

  R push(VALUE& value, const char* name=0) {
    switch (TYPE(value)) {
    case T_FIXNUM:
      return dest.pushInt(FIX2INT(value), name);
    case T_FLOAT:
      return dest.pushDouble(NUM2DBL(value), name);
    case T_STRING:
      return dest.pushString(RSTRING(value)->ptr, name);
    case T_NIL:
      return dest.pushNull(name);
    case T_TRUE:
      return dest.pushBool(true, name);
    case T_FALSE:
      return dest.pushBool(false, name);
    }
    return false;
  }
};

class RubyDest {

 public:
  RubyDest();
  ~RubyDest();

  VALUE pushString(const char* value, const char* name=0) {
    return Qnil;
  }

  VALUE pushInt(int64_t value, const char* name=0) {
    return INT2FIX(value);
  }

  VALUE pushDouble(double value, const char* name=0) {
    return rb_float_new(value);
  }

  VALUE pushBool(bool value, const char* name=0) {
    return value ? Qtrue : Qfalse;
  }

  VALUE pushNull(const char* name=0) {
    return Qnil;
  }


};


#endif
