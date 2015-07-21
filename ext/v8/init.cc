#include "rr.h"

extern "C" {
  void Init_init();
}

using namespace rr;

VALUE PropertyCallbackInfo::Class;

extern "C" {
  void Init_init() {
    V8::Init();
    DefineEnums();
    Isolate::Init();
    Handles::Init();
    Context::Init();
    Maybe::Init();
    Value::Init();
    Object::Init();
    Primitive::Init();
    Null::Init();
    Undefined::Init();
    Boolean::Init();
    Number::Init();
    Integer::Init();
    Name::Init();
    String::Init();
    Symbol::Init();
    Function::Init();
    FunctionCallbackInfo::Init();
    PropertyCallbackInfo::Init();
    ReturnValue::Init();
    Script::Init();
    ScriptOrigin::Init();
    Array::Init();
    External::Init();

    // Invocation::Init();
    // Signature::Init();
    // Date::Init();
    // Constants::Init();
    // Template::Init();
    // Stack::Init();
    // Message::Init();
    // TryCatch::Init();
    // Exception::Init();
    // ResourceConstraints::Init();
    // HeapStatistics::Init();
  }
}
