#include "rr.h"

extern "C" {
  void Init_init();
}

using namespace rr;

extern "C" {
  void Init_init() {
    Context::Init();
    String::Init();
    Script::Init();
  }
}