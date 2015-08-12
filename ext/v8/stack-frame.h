// -*- mode: c++ -*-

#ifndef RR_STACK_FRAME_H
#define RR_STACK_FRAME_H
#include "rr.h"

namespace rr {
  class StackFrame : public Ref<v8::StackFrame> {
  public:
    StackFrame(VALUE self) : Ref<v8::StackFrame>(self) {}
    StackFrame(v8::Isolate* isolate, v8::Local<v8::StackFrame> frame) :
      Ref<v8::StackFrame>(isolate, frame) {}

    static VALUE GetLineNumber(VALUE self) {
      StackFrame frame(self);
      Locker lock(frame);
      return INT2FIX(frame->GetLineNumber());
    }
    static VALUE GetColumn(VALUE self) {
      StackFrame frame(self);
      Locker lock(frame);
      return INT2FIX(frame->GetColumn());
    }
    static VALUE GetScriptId(VALUE self) {
      StackFrame frame(self);
      Locker lock(frame);
      return INT2FIX(frame->GetScriptId());
    }
    static VALUE GetScriptName(VALUE self) {
      StackFrame frame(self);
      Locker lock(frame);
      return String(frame, frame->GetScriptName());
    }
    static VALUE GetScriptNameOrSourceURL(VALUE self) {
      StackFrame frame(self);
      Locker lock(frame);
      return String(frame, frame->GetScriptNameOrSourceURL());
    }
    static VALUE GetFunctionName(VALUE self) {
      StackFrame frame(self);
      Locker lock(frame);
      return String(frame, frame->GetFunctionName());
    }
    static VALUE IsEval(VALUE self) {
      StackFrame frame(self);
      Locker lock(frame);
      return Bool(frame->IsEval());
    }
    static VALUE IsConstructor(VALUE self) {
      StackFrame frame(self);
      Locker lock(frame);
      return Bool(frame->IsConstructor());
    }

    static inline void Init() {
      ClassBuilder("StackFrame").
        defineMethod("GetLineNumber", &GetLineNumber).
        defineMethod("GetColumn", &GetColumn).
        defineMethod("GetScriptId", &GetScriptId).
        defineMethod("GetScriptName", &GetScriptName).
        defineMethod("GetScriptNameOrSourceURL", &GetScriptNameOrSourceURL).
        defineMethod("GetFunctionName", &GetFunctionName).
        defineMethod("IsEval", &IsEval).
        defineMethod("IsConstructor", &IsConstructor).
        store(&Class);
    }
  };
}

#endif /* RR_STACK_FRAME_H */
