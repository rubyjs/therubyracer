#include "v8_exception.h"
#include "rr.h"
#include "v8_handle.h"
// #include "execinfo.h"
// #include "signal.h"

using namespace v8;
namespace {

  static void* stack[20];

  void fatal(const char* location, const char* message) {
    rb_raise(rb_eFatal, "%s: %s", location, message);
  }
  // void segfault(int sig) {
  //   fprintf(stderr, "segfault: game over.\n");
  //   int size = backtrace(stack, 20);
  //   backtrace_symbols_fd(stack, size, 2);
  //   exit(1);
  // }
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

  VALUE StackTraceClass;
  VALUE StackFrameClass;
  namespace Trace {

    Persistent<StackTrace>& trace(VALUE value) {
      return rr_v8_handle<StackTrace>(value);
    }
    VALUE GetFrame(VALUE self, VALUE index) {
      HandleScope scope;
      return rr_v82rb(trace(self)->GetFrame(NUM2UINT(index)));
    }
    VALUE GetFrameCount(VALUE self) {
      HandleScope scope;
      return rr_v82rb(trace(self)->GetFrameCount());
    }
    VALUE AsArray(VALUE self) {
      return rr_v82rb(trace(self)->AsArray());
    }
    VALUE CurrentStackTrace(VALUE self, VALUE frame_limit) {
      return rr_v82rb(StackTrace::CurrentStackTrace(NUM2INT(frame_limit)));
    }
  }

  namespace Frame {
    Persistent<StackFrame>& frame(VALUE value) {
     return rr_v8_handle<StackFrame>(value);
    }
    VALUE GetLineNumber(VALUE self) {
      HandleScope scope;
      return rr_v82rb(frame(self)->GetLineNumber());
    }
    VALUE GetColumn(VALUE self) {
      HandleScope scope;
      return rr_v82rb(frame(self)->GetColumn());
    }
    VALUE GetScriptName(VALUE self) {
      HandleScope scope;
      return rr_v82rb(frame(self)->GetScriptName());
    }
    VALUE GetFunctionName(VALUE self) {
      HandleScope scope;
      return rr_v82rb(frame(self)->GetFunctionName());
    }
    VALUE IsEval(VALUE self) {
      HandleScope scope;
      return rr_v82rb(frame(self)->IsEval());
    }
    VALUE IsConstructor(VALUE self) {
      HandleScope scope;
      return rr_v82rb(frame(self)->IsConstructor());
    }
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

  StackTraceClass = rr_define_class("StackTrace", rr_v8_handle_class());
  rr_define_singleton_method(StackTraceClass, "CurrentStackTrace", Trace::CurrentStackTrace, 1);
  rr_define_method(StackTraceClass, "GetFrame", Trace::GetFrame, 1);
  rr_define_method(StackTraceClass, "GetFrameCount", Trace::GetFrameCount, 0);
  rr_define_method(StackTraceClass, "AsArray", Trace::AsArray, 0);

  StackFrameClass = rr_define_class("StackFrame", rr_v8_handle_class());
  rr_define_method(StackFrameClass, "GetLineNumber", Frame::GetLineNumber, 0);
  rr_define_method(StackFrameClass, "GetColumn", Frame::GetColumn, 0);
  rr_define_method(StackFrameClass, "GetScriptName", Frame::GetScriptName, 0);
  rr_define_method(StackFrameClass, "GetFunctionName", Frame::GetFunctionName, 0);
  rr_define_method(StackFrameClass, "IsEval", Frame::IsEval, 0);
  rr_define_method(StackFrameClass, "IsConstructor", Frame::IsConstructor, 0);

  v8::V8::SetFatalErrorHandler(fatal);
  //comment this in for debugging.
  // signal(SIGSEGV, segfault);
}

VALUE rr_reflect_v8_stacktrace(Handle<StackTrace> value) {
  return rr_v8_handle_new(StackTraceClass, value);
}
VALUE rr_reflect_v8_stackframe(Handle<StackFrame> value) {
  return rr_v8_handle_new(StackFrameClass, value);
}
