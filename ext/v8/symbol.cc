#include "rr.h"

namespace rr {
  void Symbol::Init() {
    ClassBuilder("Symbol", Name::Class).
      defineSingletonMethod("New", &New).
      defineSingletonMethod("For", &For).
      defineSingletonMethod("ForApi", &ForApi).
      defineSingletonMethod("GetIterator", &GetIterator).
      defineSingletonMethod("GetUnscopables", &GetUnscopables).
      defineSingletonMethod("GetToStringTag", &GetToStringTag).
      defineMethod("Name", &Name).

      store(&Class);
  }

  VALUE Symbol::New(int argc, VALUE argv[], VALUE self) {
    VALUE rb_isolate, rb_name;
    rb_scan_args(argc, argv, "11", &rb_isolate, &rb_name);

    Isolate isolate(rb_isolate);
    Locker lock(isolate);
    v8::HandleScope handle_scope(isolate);

    if (RTEST(rb_name)) {
      return Symbol(isolate, v8::Symbol::New(isolate, String(rb_name)));
    } else {
      return Symbol(isolate, v8::Symbol::New(isolate));
    }
  }

  VALUE Symbol::For(VALUE self, VALUE rb_isolate, VALUE name) {
    Isolate isolate(rb_isolate);
    Locker lock(isolate);

    return Symbol(isolate, v8::Symbol::For(isolate, String(name)));
  }

  VALUE Symbol::ForApi(VALUE self, VALUE rb_isolate, VALUE name) {
    Isolate isolate(rb_isolate);
    Locker lock(isolate);

    return Symbol(isolate, v8::Symbol::ForApi(isolate, String(name)));
  }

  VALUE Symbol::GetIterator(VALUE self, VALUE rb_isolate) {
    Isolate isolate(rb_isolate);
    Locker lock(isolate);

    return Symbol(isolate, v8::Symbol::GetIterator(isolate));
  }

  VALUE Symbol::GetUnscopables(VALUE self, VALUE rb_isolate) {
    Isolate isolate(rb_isolate);
    Locker lock(isolate);

    return Symbol(isolate, v8::Symbol::GetUnscopables(isolate));
  }

  VALUE Symbol::GetToStringTag(VALUE self, VALUE rb_isolate) {
    Isolate isolate(rb_isolate);
    Locker lock(isolate);

    return Symbol(isolate, v8::Symbol::GetToStringTag(isolate));
  }

  VALUE Symbol::Name(VALUE self) {
    Symbol symbol(self);
    Isolate isolate(symbol.getIsolate());
    Locker lock(isolate);

    return Value(isolate, symbol->Name());
  }
}
