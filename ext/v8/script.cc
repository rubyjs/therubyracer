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
    VALUE r_source, r_context, r_origin;
    rb_scan_args(argc, argv, "21", &r_context, &r_source, &r_origin);

    Context context(r_context);
    Isolate isolate(context.getIsolate());
    Locker lock(isolate);


    if (RTEST(r_origin)) {
      v8::ScriptOrigin origin = ScriptOrigin(r_origin);
      v8::MaybeLocal<v8::Script> script = v8::Script::Compile(context, String(r_source), &origin);
      return Script::Maybe(isolate, script);
    }
    else {
      return Script::Maybe(isolate, v8::Script::Compile(context, String(r_source)));
    }
  }

  VALUE Script::Run(VALUE self, VALUE rb_context) {
    Context context(rb_context);
    Locker lock(context);

    return Value::Maybe(context, Script(self)->Run(context));
  }
}
