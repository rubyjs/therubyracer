#include "rr.h"
#ifdef HAVE_RUBY_ENCODING_H
#include "ruby/encoding.h"
#endif

namespace rr {

void String::Init() {
  ClassBuilder("String", Value::Class).
    defineSingletonMethod("New", &New).
    defineSingletonMethod("Concat", &Concat).
    defineMethod("Utf8Value", &Utf8Value).
    store(&Class);
}

VALUE String::New(VALUE StringClass, VALUE string) {
  return String(v8::String::New(RSTRING_PTR(string), (int)RSTRING_LEN(string)));
}

VALUE String::Utf8Value(VALUE self) {
  String str(self);
  #ifdef HAVE_RUBY_ENCODING_H
  return rb_enc_str_new(*v8::String::Utf8Value(*str), str->Utf8Length(), rb_enc_find("utf-8"));
  #else
  return rb_str_new(*v8::String::Utf8Value(*str), str->Utf8Length());
  #endif
}

VALUE String::Concat(VALUE self, VALUE left, VALUE right) {
  return String(v8::String::Concat(String(left), String(right)));
}

} //namespace rr