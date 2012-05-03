#include "rr.h"

extern "C" {
  void Init_init();
}

using namespace rr;

extern "C" {
  void Init_init() {
    GC::Init();
    V8::Init();
    Context::Init();
    String::Init();
    Script::Init();
  }
}