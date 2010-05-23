#include "v8_exception.h"
#include "rr.h"
#include "execinfo.h"
#include "signal.h"

namespace {

  static void* stack[20];

  void fatal(const char* location, const char* message) {
    int size = backtrace(stack, 20);
    backtrace_symbols_fd(stack, size, 2);
    rb_raise(rb_eFatal, "%s: %s", location, message);    
  }
  void segfault(int sig) {
    fprintf(stderr, "segfault: game over.\n");
    void *elements[20];
    int size = backtrace(stack, 20);
    backtrace_symbols_fd(stack, size, 2);
    exit(1);
  }
}

void rr_init_v8_exception() {
  v8::V8::SetFatalErrorHandler(fatal);
  signal(SIGSEGV, segfault);
}