#include "rr.h"

namespace rr {

VALUE Bool(bool b) {
  return b ? Qtrue : Qfalse;
}

int Int(VALUE v) {
  return RTEST(v) ? NUM2INT(v) : 0;
}

uint32_t UInt32(VALUE num) {
  return NUM2UINT(num);
}
}