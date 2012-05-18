#include "rr.h"

namespace rr {
  void Signature::Init() {
    ClassBuilder("Signature").
      defineMethod("New", &New).
      store(&Class);
  }

  VALUE Signature::New(int length, VALUE args[], VALUE self) {
    VALUE receiver; VALUE argc; VALUE argv;
    rb_scan_args(length, args, "03", &receiver, &argc, &argv);
    std::vector< v8::Handle<v8::FunctionTemplate> > parameters(Int(argc));
    return Signature(v8::Signature::New(FunctionTemplate(receiver), Int(argc), FunctionTemplate::array(argv, parameters)));
  }
}