#include "rr.h"

namespace rr {
  VALUE Signature::New(int argc, VALUE argv[], VALUE self) {
    VALUE r_isolate;
    VALUE r_receiver;
    rb_scan_args(argc, argv, "11", &r_isolate, &r_receiver);

    Isolate isolate(r_isolate);
    Locker lock(isolate);
    return Signature(isolate, v8::Signature::New(isolate, FunctionTemplate(r_receiver)));
  }
}
