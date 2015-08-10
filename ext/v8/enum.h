#include "rr.h"

#ifndef ENUM_H
#define ENUM_H

namespace rr {

  template <class T>
  class Enum {
    VALUE value;
    T defaultValue;

  public:
    Enum(VALUE value_, T defaultValue_) : value(value_), defaultValue(defaultValue_) { }

    operator T() {
      return RTEST(value) ? (T)NUM2INT(value) : defaultValue;
    }

    class Maybe : public rr::Maybe {
    public:
      Maybe(v8::Maybe<T> maybe) {
        if (maybe.IsJust()) {
          just(INT2FIX((int)maybe.FromJust()));
        }
      }
    };
  };

  inline void DefineEnums() {
    ClassBuilder("AccessControl").
      defineConst("DEFAULT", INT2FIX(v8::DEFAULT)).
      defineConst("ALL_CAN_READ", INT2FIX(v8::ALL_CAN_READ)).
      defineConst("ALL_CAN_WRITE", INT2FIX(v8::ALL_CAN_WRITE)).
      defineConst("PROHIBITS_OVERWRITING", INT2FIX(v8::PROHIBITS_OVERWRITING));

    ClassBuilder("PropertyAttribute").
      defineConst("None", INT2FIX(v8::None)).
      defineConst("ReadOnly", INT2FIX(v8::ReadOnly)).
      defineConst("DontEnum", INT2FIX(v8::DontEnum)).
      defineConst("DontDelete", INT2FIX(v8::DontDelete));
  }

};

#endif /* ENUM_H */
