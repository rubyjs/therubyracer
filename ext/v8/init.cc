#include "rr.h"

extern "C" {
  void Init_init();
}

using namespace rr;

extern "C" {
  void Init_init() {
    V8::Init();
    Isolate::Init();
    Handles::Init();
    Context::Init();
    Backref::Init();
    Value::Init();
    Object::Init();
    Primitive::Init();
    String::Init();
    Function::Init();
    Script::Init();
    Array::Init();

    // Accessor::Init();
    // Invocation::Init();
    // Signature::Init();
    // Date::Init();
    // Constants::Init();
    // External::Init();
    // Template::Init();
    // Stack::Init();
    // Message::Init();
    // TryCatch::Init();
    // Exception::Init();
    // ResourceConstraints::Init();
    // HeapStatistics::Init();
  }
}
