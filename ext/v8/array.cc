#include "rr.h"

namespace rr {

void Array::Init() {
  ClassBuilder("Array", Object::Class).
    store(&Class);
}

}