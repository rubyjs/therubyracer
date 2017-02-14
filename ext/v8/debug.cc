#include "rr.h"
#include <v8-debug.h>

namespace rr {

void Debug::Init() {
  ClassBuilder("Debug", Value::Class).
    defineSingletonMethod("EnableAgent", &EnableAgent);
}

VALUE Debug::EnableAgent(VALUE self, 
    VALUE application_name, 
    VALUE port, 
    VALUE wait_for_connection) {
  return Bool( v8::Debug::EnableAgent( RSTRING_PTR(application_name), 
        NUM2INT(port), 
        RTEST(wait_for_connection)
        )
      );
}

} //namespace rr
