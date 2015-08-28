#include "rr.h"

extern "C" {
  void Init_init();
}

using namespace rr;

VALUE PropertyCallbackInfo::Class;
VALUE ReturnValue::Class;

extern "C" {
  void Init_init() {
    rb_eval_string("require 'v8/c'");

    V8::Init();
    DefineEnums();
    Isolate::Init();
    Handle::Init();
    Handles::Init();
    Context::Init();
    Maybe::Init();
    Value::Init();
    Object::Init();
    Date::Init();
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
    Template::Init();
    ObjectTemplate::Init();
    FunctionTemplate::Init();
    Signature::Init();
    StackFrame::Init();
    StackTrace::Init();
    Message::Init();
    Exception::Init();
    TryCatch::Init();

    // Invocation::Init();
    // Constants::Init();
    // Template::Init();
    // ResourceConstraints::Init();
    // HeapStatistics::Init();
  }
}
