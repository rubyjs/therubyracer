#include "rr.h"

namespace rr {

void V8::Init() {
  ClassBuilder("V8").
    defineSingletonMethod("IdleNotification", &IdleNotification).
    defineSingletonMethod("SetCaptureStackTraceForUncaughtExceptions", &SetCaptureStackTraceForUncaughtExceptions);
}

VALUE V8::IdleNotification(int argc, VALUE argv[], VALUE self) {
  VALUE hint;
  rb_scan_args(argc, argv, "01", &hint);
  if (RTEST(hint)) {
    return Bool(v8::V8::IdleNotification(NUM2INT(hint)));
  } else {
    return Bool(v8::V8::IdleNotification());
  }
}

VALUE V8::SetCaptureStackTraceForUncaughtExceptions(int argc, VALUE argv[], VALUE self) {
  VALUE should_capture; VALUE frame_limit; VALUE options;
  rb_scan_args(argc, argv, "12", &should_capture, &frame_limit, &options);
  int limit = RTEST(frame_limit) ? NUM2INT(frame_limit) : 10;
  Void(v8::V8::SetCaptureStackTraceForUncaughtExceptions(Bool(should_capture), limit, Stack::Trace::StackTraceOptions(options)));
}

}