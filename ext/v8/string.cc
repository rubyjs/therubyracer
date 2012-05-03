#include "rr.h"

namespace rr {
namespace {
  VALUE New(VALUE StringClass, VALUE string) {
    v8::HandleScope h;
    return String::ToRuby(v8::String::New(RSTRING_PTR(string), (int)RSTRING_LEN(string)));
  }
  VALUE Utf8Value(VALUE self) {
    v8::HandleScope h;
    String str(self);
    return rb_str_new(*v8::String::Utf8Value(str), str->Utf8Length());
  }
}

VALUE String::ToRuby(v8::Handle<v8::String> string) {
  return String::create(string, String::Class);
}

String::operator v8::Handle<v8::Value>() {
  return this->GetHandle();
}

VALUE String::Class;

void String::Init() {
  rb_gc_register_address(&Class);
  Class = defineClass("String");
  RR_DEFINE_SINGLETON_METHOD(Class, "New", &New, 1);
  RR_DEFINE_METHOD(Class, "Utf8Value", &Utf8Value, 0);
}
}