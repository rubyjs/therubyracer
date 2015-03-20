#ifndef RR_UINT32
#define RR_UINT32

namespace rr {

  class UInt32 : public Equiv {
  public:
    UInt32(VALUE val) : Equiv(val) {}
    UInt32(uint32_t ui) : Equiv(UINT2NUM(ui)) {}

    inline operator uint32_t() {
      return RTEST(value) ? NUM2UINT(value) : 0;
    }
  };

}

#endif
