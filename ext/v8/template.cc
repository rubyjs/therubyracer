#include "rr.h"

namespace rr {
  void Template::Init() {
    ClassBuilder("Template");
    FunctionTemplate::Init();
  }

  void FunctionTemplate::Init() {
    ClassBuilder("FunctionTemplate", "Template").
      store(&Class);
  }
}