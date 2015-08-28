namespace rr {
  class V8 {
  public:
    static v8::Platform* v8_platform;

    static void Init();
    // static VALUE IdleNotification(int argc, VALUE argv[], VALUE self);
    static VALUE SetFlagsFromString(VALUE self, VALUE string);
    // static VALUE SetFlagsFromCommandLine(VALUE self, VALUE args, VALUE remove_flags);
    // static VALUE AdjustAmountOfExternalAllocatedMemory(VALUE self, VALUE change_in_bytes);
    // static VALUE PauseProfiler(VALUE self);
    // static VALUE ResumeProfiler(VALUE self);
    // static VALUE IsProfilerPaused(VALUE self);
    // static VALUE GetCurrentThreadId(VALUE self);
    // static VALUE TerminateExecution(VALUE self, VALUE thread_id);
    // static VALUE IsExecutionTerminating(VALUE self);
    static VALUE Dispose(VALUE self);
    // static VALUE LowMemoryNotification(VALUE self);
    // static VALUE ContextDisposedNotification(VALUE self);

    // static VALUE SetCaptureStackTraceForUncaughtExceptions(int argc, VALUE argv[], VALUE self);
    // static VALUE GetHeapStatistics(VALUE self, VALUE statistics_ptr);
    static VALUE GetVersion(VALUE self);
  };
}
