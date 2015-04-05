#include "rr.h"

namespace rr {

  v8::Platform* V8::v8_platform = NULL;

  void V8::Init() {
    // Initialize V8.
    v8::V8::InitializeICU();
    v8_platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(v8_platform);
    v8::V8::Initialize();

    ClassBuilder("V8").
      // defineSingletonMethod("IdleNotification", &IdleNotification).
      // defineSingletonMethod("SetFlagsFromString", &SetFlagsFromString).
      // defineSingletonMethod("SetFlagsFromCommandLine", &SetFlagsFromCommandLine).
      // defineSingletonMethod("PauseProfiler", &PauseProfiler).
      // defineSingletonMethod("ResumeProfiler", &ResumeProfiler).
      // defineSingletonMethod("IsProfilerPaused", &IsProfilerPaused).
      // defineSingletonMethod("GetCurrentThreadId", &GetCurrentThreadId).
      // defineSingletonMethod("TerminateExecution", &TerminateExecution).
      // defineSingletonMethod("IsExecutionTerminating", &IsExecutionTerminating).
      defineSingletonMethod("Dispose", &Dispose).
      // defineSingletonMethod("LowMemoryNotification", &LowMemoryNotification).
      // defineSingletonMethod("AdjustAmountOfExternalAllocatedMemory", &AdjustAmountOfExternalAllocatedMemory).
      // defineSingletonMethod("ContextDisposedNotification", &ContextDisposedNotification).
      // defineSingletonMethod("SetCaptureStackTraceForUncaughtExceptions", &SetCaptureStackTraceForUncaughtExceptions).
      // defineSingletonMethod("GetHeapStatistics", &GetHeapStatistics).
      defineSingletonMethod("GetVersion", &GetVersion);
  }

  VALUE V8::Dispose(VALUE self) {
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();

    if (v8_platform) {
      delete v8_platform;
      v8_platform = NULL;
    }

    return Qnil;
  }

  VALUE V8::GetVersion(VALUE self) {
    return rb_str_new2(v8::V8::GetVersion());
  }

}
