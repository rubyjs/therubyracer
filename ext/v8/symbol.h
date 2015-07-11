// -*- mode: c++ -*-
#ifndef SYMBOL_H
#define SYMBOL_H

#include "rr.h"

namespace rr {
  class Symbol : public Ref<v8::Symbol> {
  public:
    static void Init();

    static VALUE Name(VALUE self);
    static VALUE New(int argc, VALUE argv[], VALUE self);
    static VALUE For(VALUE self, VALUE isolate, VALUE name);
    static VALUE ForApi(VALUE self, VALUE isolate, VALUE name);
    static VALUE GetIterator(VALUE self, VALUE isolate);
    static VALUE GetUnscopables(VALUE self, VALUE isolate);
    static VALUE GetToStringTag(VALUE self, VALUE isolate);

    Symbol(VALUE self) : Ref<v8::Symbol>(self) {}
    Symbol(v8::Isolate* isolate, v8::Local<v8::Symbol> symbol) :
      Ref<v8::Symbol>(isolate, symbol) {}
  };
}

#endif /* SYMBOL_H */
