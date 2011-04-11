#include "v8_message.h"
#include "v8_handle.h"

using namespace v8;

namespace {
  VALUE MessageClass;
  
  Persistent<Message>& unwrap(VALUE self) {
    return rr_v8_handle<Message>(self);
  }
  
  VALUE Get(VALUE self) {
    return rr_v82rb(unwrap(self)->Get());
  }

  VALUE GetSourceLine(VALUE self) {
    return rr_v82rb(unwrap(self)->GetSourceLine());
  }

  VALUE GetScriptResourceName(VALUE self) {
    return rr_v82rb(unwrap(self)->GetScriptResourceName());
  }

  VALUE GetStackTrace(VALUE self) {
    Handle<StackTrace> trace = unwrap(self)->GetStackTrace();
    return trace.IsEmpty() ? Qnil : rr_v82rb(trace);
  }

  VALUE GetLineNumber(VALUE self) {
    return rr_v82rb(unwrap(self)->GetLineNumber());
  }

  VALUE GetStartPosition(VALUE self) {
    return rr_v82rb(unwrap(self)->GetStartPosition());
  }

  VALUE GetEndPosition(VALUE self) {
    return rr_v82rb(unwrap(self)->GetEndPosition());
  }

  VALUE GetStartColumn(VALUE self) {
    return rr_v82rb(unwrap(self)->GetStartColumn());
  }

  VALUE GetEndColumn(VALUE self) {
    return rr_v82rb(unwrap(self)->GetEndColumn());
  }
}

void rr_init_message() {
  MessageClass = rr_define_class("Message", rr_v8_handle_class());
  rr_define_method(MessageClass, "Get", Get, 0);
  rr_define_method(MessageClass, "GetSourceLine", GetSourceLine, 0);
  rr_define_method(MessageClass, "GetScriptResourceName", GetScriptResourceName, 0);
  rr_define_method(MessageClass, "GetStackTrace", GetStackTrace, 0);
  rr_define_method(MessageClass, "GetLineNumber", GetLineNumber, 0);
  rr_define_method(MessageClass, "GetStartPosition", GetStartPosition, 0);
  rr_define_method(MessageClass, "GetEndPosition", GetEndPosition, 0);
  rr_define_method(MessageClass, "GetStartColumn", GetStartColumn, 0);
  rr_define_method(MessageClass, "GetEndColumn", GetEndColumn, 0);  
}

VALUE rr_reflect_v8_message(Handle<Message> value) {
  return rr_v8_handle_new(MessageClass, value);
}

