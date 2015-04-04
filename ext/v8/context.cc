#include "rr.h"

namespace rr {

  void Context::Init() {
    ClassBuilder("Context").
      defineSingletonMethod("New", &New).

      defineMethod("Dispose", &Dispose).
      defineMethod("Enter", &Enter).
      defineMethod("Exit", &Exit).

      defineMethod("Global", &Global).

      store(&Class);

    // TODO
    // ClassBuilder("ExtensionConfiguration").
    //   defineSingletonMethod("new", &ExtensionConfiguration::initialize).
    //   store(&ExtensionConfiguration::Class);
  }

  VALUE Context::New(int argc, VALUE argv[], VALUE self) {
    VALUE rb_isolate, extension_configuration, global_template, global_object;
    rb_scan_args(argc, argv, "13", &rb_isolate, &extension_configuration, &global_template, &global_object);

    Isolate isolate(rb_isolate);
    Locker lock(isolate);

    return Context(v8::Context::New(
      isolate
      // TODO
      // ,
      // ExtensionConfiguration(extension_configuration),
      // *ObjectTemplate(global_template),
      // *Object(global_object)
    ));
  }

  VALUE Context::Dispose(VALUE self) {
    Context(self).dispose();
    return Qnil;
  }

  VALUE Context::Enter(VALUE self) {
    Context context(self);
    Locker lock(context.getIsolate());

    context->Enter();

    return Qnil;
  }

  VALUE Context::Exit(VALUE self) {
    Context context(self);
    Locker lock(context.getIsolate());

    context->Exit();

    return Qnil;
  }

  VALUE Context::Global(VALUE self) {
    Context context(self);
    Locker lock(context.getIsolate());

    return Object(context->GetIsolate(), context->Global());
  }

  // TODO
  // template <> void Pointer<v8::ExtensionConfiguration>::unwrap(VALUE value) {
  //   Data_Get_Struct(value, class v8::ExtensionConfiguration, pointer);
  // }

}
