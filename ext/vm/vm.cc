
#include "rr.h"

extern "C" {
  void Init_vm();
}

extern "C" {
  void Init_vm() {
    v8::Locker locker;
  }
}
