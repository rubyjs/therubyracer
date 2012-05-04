#include "rr.h"

namespace rr {

VALUE String::Class;

void String::Init() {
  rb_gc_register_address(&Class);
  Class = ClassBuilder("String", "Value").
    defineSingletonMethod("New", &New).
    defineMethod("Utf8Value", &Utf8Value);
}

VALUE String::New(VALUE StringClass, VALUE string) {
  v8::HandleScope h;
  return String::Convert(v8::String::New(RSTRING_PTR(string), (int)RSTRING_LEN(string)));
}

VALUE String::Utf8Value(VALUE self) {
  v8::HandleScope h;
  String str(self);
  return rb_str_new(*v8::String::Utf8Value(str.GetHandle()), str->Utf8Length());
}

VALUE String::Convert(v8::Handle<v8::String> string) {
  return String::create(string, String::Class);
}

} //namespace rr