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
    Locker::Init();

    // Accessor::Init();
    // Invocation::Init();
    // Signature::Init();
    // Array::Init();
    // Function::Init();
    // Date::Init();
    // Constants::Init();
    // External::Init();
    // Script::Init();
    // Template::Init();
    // Stack::Init();
    // Message::Init();
    // TryCatch::Init();
    // Exception::Init();
    // ResourceConstraints::Init();
    // HeapStatistics::Init();
  }
}
