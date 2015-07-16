#include "rr.h"

namespace rr {

  class Enums {
  public:
    static void Init() {
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

    template <class T>
    static T fromRubyValue(VALUE rb_int) {
      return (T)NUM2INT(rb_int);
    }
  };

};
