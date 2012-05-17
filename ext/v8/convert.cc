#include "rr.h"

namespace rr {

VALUE Bool(bool b) {
  return b ? Qtrue : Qfalse;
}

VALUE Int(int i) {
  return INT2FIX(i);
}

uint32_t UInt32(VALUE num) {
  return NUM2UINT(num);
}
}