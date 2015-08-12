// -*- mode: c++ -*-
#ifndef RR_STACK_TRACE_H
#define RR_STACK_TRACE_H
#include "rr.h"

namespace rr {
  class StackTrace : public Ref<v8::StackTrace> {
  public:
    StackTrace(VALUE self) : Ref<v8::StackTrace>(self) {}
    StackTrace(v8::Isolate* isolate, v8::Local<v8::StackTrace> trace) :
      Ref<v8::StackTrace>(isolate, trace) {}

    static VALUE GetFrame(VALUE self, VALUE offset) {
      StackTrace stack(self);
      Locker lock(stack);

      return StackFrame(stack, stack->GetFrame(NUM2INT(offset)));
    }

    static VALUE GetFrameCount(VALUE self) {
      StackTrace stack(self);
      Locker lock(stack);

      return INT2FIX(stack->GetFrameCount());
    }

    static VALUE AsArray(VALUE self) {
      StackTrace stack(self);
      Locker lock(stack);

      return Array(stack, stack->AsArray());
    }

    static VALUE CurrentStackTrace(VALUE self, VALUE rb_isolate, VALUE frame_limit, VALUE options) {
      Isolate isolate(rb_isolate);
      Locker lock(isolate);

      return StackTrace(
        isolate,
        v8::StackTrace::CurrentStackTrace(
          isolate,
          NUM2UINT(frame_limit),
          Enum<v8::StackTrace::StackTraceOptions>(options, v8::StackTrace::kOverview)));
    }

    inline static void Init() {
      ClassBuilder("StackTrace").
        defineMethod("GetFrame", &GetFrame).
        defineMethod("GetFrameCount", &GetFrameCount).
        defineMethod("AsArray", &AsArray).
        defineSingletonMethod("CurrentStackTrace", &CurrentStackTrace).

        defineConstMethod("kLineNumber", INT2FIX(v8::StackTrace::kLineNumber)).
        defineConstMethod("kColumnOffset", INT2FIX(v8::StackTrace::kColumnOffset)).
        defineConstMethod("kScriptName", INT2FIX(v8::StackTrace::kScriptName)).
        defineConstMethod("kFunctionName", INT2FIX(v8::StackTrace::kFunctionName)).
        defineConstMethod("kIsEval", INT2FIX(v8::StackTrace::kIsEval)).
        defineConstMethod("kIsConstructor", INT2FIX(v8::StackTrace::kIsConstructor)).
        defineConstMethod("kScriptNameOrSourceURL", INT2FIX(v8::StackTrace::kScriptNameOrSourceURL)).
        defineConstMethod("kScriptId", INT2FIX(v8::StackTrace::kScriptId)).
        defineConstMethod("kExposeFramesAcrossSecurityOrigins", INT2FIX(v8::StackTrace::kExposeFramesAcrossSecurityOrigins)).
        defineConstMethod("kOverview", INT2FIX(v8::StackTrace::kOverview)).
        defineConstMethod("kDetailed", INT2FIX(v8::StackTrace::kDetailed)).
        store(&Class);
    }
  };

}

#endif /* RR_STACK_TRACE_H */
