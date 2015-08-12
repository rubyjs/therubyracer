// -*- mode: c++ -*-

#ifndef RR_MESSAGE_H
#define RR_MESSAGE_H
#include "rr.h"

namespace rr {
  class Message : public Ref<v8::Message> {
  public:
    Message(VALUE self) : Ref<v8::Message>(self) {}
    Message(v8::Isolate* isolate, v8::Local<v8::Message> msg) :
      Ref<v8::Message>(isolate, msg) {}

    static VALUE Get(VALUE self) {
      Message message(self);
      Locker lock(message);
      return String(message, message->Get());
    }
    static VALUE GetSourceLine(VALUE self, VALUE cxt) {
      Message message(self);
      Locker lock(message);
      return String::Maybe(message, message->GetSourceLine(Context(cxt)));
    }
    static VALUE GetScriptOrigin(VALUE self) {
      Message message(self);
      Locker lock(message);
      return ScriptOrigin(message, message->GetScriptOrigin());
    }
    static VALUE GetScriptResourceName(VALUE self) {
      Message message(self);
      Locker lock(message);
      return Value(message, message->GetScriptResourceName());
    }
    static VALUE GetStackTrace(VALUE self) {
      Message message(self);
      Locker lock(message);
      return StackTrace(message, message->GetStackTrace());
    }
    static VALUE GetLineNumber(VALUE self, VALUE cxt) {
      Message message(self);
      Locker lock(message);
      return Int::Maybe(message->GetLineNumber(Context(cxt)));
    }
    static VALUE GetStartPosition(VALUE self) {
      Message message(self);
      Locker lock(message);
      return Int(message->GetStartPosition());
    }
    static VALUE GetEndPosition(VALUE self) {
      Message message(self);
      Locker lock(message);
      return Int(message->GetEndPosition());
    }
    static VALUE GetStartColumn(VALUE self, VALUE cxt) {
      Message message(self);
      Locker lock(message);
      return Int::Maybe(message->GetStartColumn(Context(cxt)));
    }
    static VALUE GetEndColumn(VALUE self, VALUE cxt) {
      Message message(self);
      Locker lock(message);
      return Int::Maybe(message->GetEndColumn(Context(cxt)));
    }
    static VALUE IsOpaque(VALUE self) {
      Message message(self);
      Locker lock(message);
      return Bool(message->IsOpaque());
    }
    static VALUE IsSharedCrossOrigin(VALUE self) {
      Message message(self);
      Locker lock(message);
      return Bool(message->IsSharedCrossOrigin());
    }

    static inline void Init() {
      ClassBuilder("Message").
        defineMethod("Get", &Get).
        defineMethod("GetSourceLine", &GetSourceLine).
        defineMethod("GetScriptOrigin", &GetScriptOrigin).
        defineMethod("GetScriptResourceName", &GetScriptResourceName).
        defineMethod("GetStackTrace", &GetStackTrace).
        defineMethod("GetLineNumber", &GetLineNumber).
        defineMethod("GetStartPosition", &GetStartPosition).
        defineMethod("GetEndPosition", &GetEndPosition).
        defineMethod("GetStartColumn", &GetStartColumn).
        defineMethod("GetEndColumn", &GetEndColumn).
        defineMethod("IsSharedCrossOrigin", &IsSharedCrossOrigin).
        defineMethod("IsOpaque", &IsOpaque).
        store(&Class);
    }
  };
}


#endif /* RR_MESSAGE_H */
