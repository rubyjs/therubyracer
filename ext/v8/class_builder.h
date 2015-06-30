// -*- mode: c++ -*-
#ifndef RR_CLASS_BUILDER
#define RR_CLASS_BUILDER

namespace rr {

  class ClassBuilder {
  public:
    static VALUE defineClass(const char *name, VALUE superclass = rb_cObject);
    static VALUE defineModule(const char *name);

    ClassBuilder() {};
    ClassBuilder(const char* name, VALUE superclass = rb_cObject);
    ClassBuilder(const char* name, const char* supername);

    ClassBuilder& defineConst(const char* name, VALUE value);

    ClassBuilder& defineMethod(const char* name, VALUE (*impl)(int, VALUE*, VALUE));
    ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE));
    ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE, VALUE));
    ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE, VALUE, VALUE));
    ClassBuilder& defineMethod(const char* name, VALUE (*impl)(VALUE, VALUE, VALUE, VALUE));

    ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(int, VALUE*, VALUE));
    ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE));
    ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE, VALUE));
    ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE, VALUE, VALUE));
    ClassBuilder& defineSingletonMethod(const char* name, VALUE (*impl)(VALUE, VALUE, VALUE, VALUE));

    ClassBuilder& defineEnumConst(const char* name, int value);

    ClassBuilder& store(VALUE* storage);

    inline operator VALUE() {return this->value;}
  protected:
    VALUE value;
  };

}

#endif
