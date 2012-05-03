#include "rr.h"

namespace rr {
  VALUE StringClass;
namespace {
  VALUE New(VALUE StringClass, VALUE string) {
    v8::HandleScope h;
    return String::ToRuby(v8::String::New(RSTRING_PTR(string), (int)RSTRING_LEN(string)));
  }
}

VALUE String::ToRuby(v8::Handle<v8::String> string) {
  return String::create(string, StringClass);
}

void String::Init() {
  rb_gc_register_address(&StringClass);
  StringClass = defineClass("String");
  RR_DEFINE_SINGLETON_METHOD(StringClass, "New", &New, 1);
}
}