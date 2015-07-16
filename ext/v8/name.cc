#include "rr.h"

namespace rr {
  void Name::Init() {
    ClassBuilder("Name", Primitive::Class).
      defineMethod("GetIdentityHash", &GetIdentityHash).

      store(&Class);
  }
  VALUE Name::GetIdentityHash(VALUE self) {
    Name name(self);
    Locker lock(name.getIsolate());

    return INT2FIX(name->GetIdentityHash());
  }
}
