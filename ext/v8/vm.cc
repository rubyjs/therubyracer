#include "rr.h"

extern "C" {
  void Init_vm();
}

using namespace rr;

extern "C" {
  void Init_vm() {
    Context::Initialize();
  }
}