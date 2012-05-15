#include "rr.h"

namespace rr {

VALUE Bool(bool b) {
  return b ? Qtrue : Qfalse;
}

Value Int(int i) {
  return INT2FIX(i);
}
}