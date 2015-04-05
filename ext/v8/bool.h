#ifndef RR_BOOL
#define RR_BOOL

namespace rr {

  class Bool : public Equiv {
  public:
    Bool(VALUE val) : Equiv(val) {}
    Bool(bool b) : Equiv(b ? Qtrue : Qfalse) {}
    Bool(v8::Handle<v8::Boolean> b) : Equiv(b->Value() ? Qtrue : Qfalse) {}

    inline operator bool() {
      return RTEST(value);
    }
  };

}

#endif
