#include "rr.h"

namespace rr {
  void Script::Init() {
    ClassBuilder("Script").
      defineSingletonMethod("Compile", &Compile).

      defineMethod("Run", &Run).
      // TODO
      // defineMethod("RunWithTimeout", &RunWithTimeout).

      store(&Class);

    // TODO
    // ClassBuilder("ScriptData").
    //   defineSingletonMethod("PreCompile", &ScriptData::PreCompile).
    //   defineSingletonMethod("New", &ScriptData::New).
    //   defineMethod("Length", &ScriptData::Length).
    //   defineMethod("Data", &ScriptData::Data).
    //   defineMethod("HasError", &ScriptData::HasError).
    //   store(&ScriptData::Class);
  }


  VALUE Script::Compile(int argc, VALUE argv[], VALUE self) {
    VALUE source, rb_context, origin;
    rb_scan_args(argc, argv, "21", &source, &rb_context, &origin);

    Context context(rb_context);
    Locker lock(context.getIsolate());

    // TODO: ScriptOrigin
    return Script(context.getIsolate(), v8::Script::Compile(String(source)));
  }

  VALUE Script::Run(VALUE self, VALUE rb_context) {
    Context context(rb_context);
    Locker lock(context->GetIsolate());

    return Value::handleToRubyObject(context->GetIsolate(), Script(self)->Run());
  }
}
