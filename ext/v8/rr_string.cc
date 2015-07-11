#include "rr.h"

namespace rr {

  void String::Init() {
    ClassBuilder("String", Name::Class).
      defineSingletonMethod("NewFromUtf8", &NewFromUtf8).
      defineSingletonMethod("Concat", &Concat).

      defineMethod("Utf8Value", &Utf8Value).

      store(&Class);
  }

  VALUE String::NewFromUtf8(VALUE StringClass, VALUE rb_isolate, VALUE string) {
    Isolate isolate(rb_isolate);
    Locker lock(isolate);

    v8::Local<v8::String> v8_string = v8::String::NewFromUtf8(isolate, RSTRING_PTR(string), v8::String::kNormalString, (int)RSTRING_LEN(string));

    return String(isolate, v8_string);
  }

  VALUE String::Utf8Value(VALUE self) {
    String string(self);
    Locker lock(string.getIsolate());

    #ifdef HAVE_RUBY_ENCODING_H
    return rb_enc_str_new(*v8::String::Utf8Value(*string), string->Utf8Length(), rb_enc_find("utf-8"));
    #else
    return rb_str_new(*v8::String::Utf8Value(*string), string->Utf8Length());
    #endif
  }

  VALUE String::Concat(VALUE self, VALUE left, VALUE right) {
    String left_string(left);
    Locker lock(left_string.getIsolate());

    return String(left_string.getIsolate(), v8::String::Concat(left_string, String(right)));
  }

  String::operator v8::Handle<v8::String>() const {
    Locker lock(getIsolate());

    switch (TYPE(value)) {
    case T_STRING:
      return v8::String::NewFromUtf8(getIsolate(), RSTRING_PTR(value), v8::String::kNormalString, (int)RSTRING_LEN(value));
    case T_DATA:
      return Ref<v8::String>::operator v8::Handle<v8::String>();
    default:
      VALUE string = rb_funcall(value, rb_intern("to_s"), 0);
      return v8::String::NewFromUtf8(getIsolate(), RSTRING_PTR(string), v8::String::kNormalString, (int)RSTRING_LEN(string));
    }
  }

}
