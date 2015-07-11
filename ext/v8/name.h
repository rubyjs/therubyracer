//mode -*- c++ -*-
#ifndef NAME_H
#define NAME_H

namespace rr {
  class Name : public Ref<v8::Name> {
  public:
    static void Init();
    static VALUE GetIdentityHash(VALUE self);

    Name(VALUE self) : Ref<v8::Name>(self) {}
  };
}

#endif /* NAME_H */
