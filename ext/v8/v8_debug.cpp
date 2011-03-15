#include "rr.h"
#include "v8_debug.h"
#include "v8-debug.h"

using namespace v8;

namespace {

  VALUE EnableAgent(VALUE self, VALUE application_name, VALUE port) {
    return rr_v82rb(v8::Debug::EnableAgent(RSTRING_PTR(application_name), NUM2INT(port), false));
  }
}

void rr_init_v8_debug() {
  VALUE DebugModule = rr_define_module("Debug");
  rr_define_singleton_method(DebugModule, "EnableAgent", EnableAgent, 2);
}
