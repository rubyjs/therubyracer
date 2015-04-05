#ifndef RR_EQUIV
#define RR_EQUIV

namespace rr {

  class Equiv {
  public:
    Equiv(VALUE val) : value(val) {}
    inline operator VALUE() { return value; }

  protected:
    VALUE value;
  };

}

#endif
