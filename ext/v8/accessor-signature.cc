#include "rr.h"

namespace rr {
  VALUE AccessorSignature::New(int argc, VALUE argv[], VALUE self) {
    VALUE r_isolate;
    VALUE r_receiver;
    rb_scan_args(argc, argv, "11", &r_isolate, &r_receiver);

    Isolate isolate(r_isolate);
    Locker lock(isolate);
    return AccessorSignature(isolate, v8::AccessorSignature::New(isolate, FunctionTemplate(r_receiver)));
  }
}
