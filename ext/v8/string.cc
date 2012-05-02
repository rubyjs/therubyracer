#include "rr.h"

namespace rr {
  String::String(VALUE object) : Value(object) {}
  String::operator v8::Handle<v8::String>() {
    return v8::String::New(RSTRING_PTR(object), (int)RSTRING_LEN(object));
  }

namespace {
  VALUE New(VALUE StringClass, VALUE string) {
    return Ref<v8::String>::create(String(string), StringClass);
  }
}

void String::Init() {
  VALUE StringClass = defineClass("String");
  RR_DEFINE_SINGLETON_METHOD(StringClass, "New", &New, 1);
}
}