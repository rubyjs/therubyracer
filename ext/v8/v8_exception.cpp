#include "v8_exception.h"
#include "rr.h"
#include "execinfo.h"
#include "signal.h"

using namespace v8;
namespace {

  static void* stack[20];

  void fatal(const char* location, const char* message) {
    rb_raise(rb_eFatal, "%s: %s", location, message);    
  }
  void segfault(int sig) {
    fprintf(stderr, "segfault: game over.\n");
    int size = backtrace(stack, 20);
    backtrace_symbols_fd(stack, size, 2);
    exit(1);
  }
  VALUE _ThrowException(VALUE rbmod, VALUE value) {
    HandleScope scope;
    Handle<Value> err = rr_rb2v8(value);
    return rr_v82rb(ThrowException(err));
  }
  VALUE RangeError(VALUE rbclass, VALUE value) {
    HandleScope scope;
    return rr_v82rb(Exception::RangeError(rr_rb2v8(value)->ToString()));
  }
  VALUE ReferenceError(VALUE rbclass, VALUE value) {
    HandleScope scope;
    return rr_v82rb(Exception::ReferenceError(rr_rb2v8(value)->ToString()));
  }
  VALUE SyntaxError(VALUE rbclass, VALUE value) {
    HandleScope scope;
    return rr_v82rb(Exception::SyntaxError(rr_rb2v8(value)->ToString()));
  }
  VALUE Error(VALUE rbclass, VALUE value) {
    HandleScope scope;
    return rr_v82rb(Exception::Error(rr_rb2v8(value)->ToString()));
  }
}

void rr_init_v8_exception() {
  VALUE V8 = rb_define_module("V8");
  VALUE V8_C = rb_define_module_under(V8, "C");
  rr_define_singleton_method(V8_C, "ThrowException", _ThrowException, 1);
  VALUE ExceptionClass = rr_define_class("Exception");
  rr_define_singleton_method(ExceptionClass, "RangeError", RangeError, 1);
  rr_define_singleton_method(ExceptionClass, "ReferenceError", ReferenceError, 1);
  rr_define_singleton_method(ExceptionClass, "SyntaxError", SyntaxError, 1);
  rr_define_singleton_method(ExceptionClass, "Error", Error, 1);
  
  v8::V8::SetFatalErrorHandler(fatal);
  signal(SIGSEGV, segfault);
}