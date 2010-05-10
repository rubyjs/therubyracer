#include "converters.h"
#include "v8_msg.h"
#include "v8_ref.h"

using namespace v8;

VALUE V8_C_Message;

void rr_init_msg() {
  V8_C_Message = rr_define_class("Message");
  rb_define_method(V8_C_Message, "Get", (VALUE(*)(...))v8_Message_Get, 0);
  rb_define_method(V8_C_Message, "GetSourceLine", (VALUE(*)(...))v8_Message_GetSourceLine, 0);
  rb_define_method(V8_C_Message, "GetScriptResourceName", (VALUE(*)(...))v8_Message_GetScriptResourceName, 0);
  rb_define_method(V8_C_Message, "GetLineNumber", (VALUE(*)(...))v8_Message_GetLineNumber, 0);
  rb_define_method(V8_C_Message, "GetStartPosition", (VALUE(*)(...))v8_Message_GetStartPosition, 0);
  rb_define_method(V8_C_Message, "GetEndPosition", (VALUE(*)(...)) v8_Message_GetEndPosition, 0);
  rb_define_method(V8_C_Message, "GetStartColumn", (VALUE(*)(...)) v8_Message_GetStartColumn, 0);
  rb_define_method(V8_C_Message, "GetEndColumn", (VALUE(*)(...)) v8_Message_GetEndColumn, 0);  
}

VALUE V8_Wrap_Message(Handle<v8::Message> msg) {
  return V8_Ref_Create(V8_C_Message, msg);
}

VALUE v8_Message_Get(VALUE self) {
  Local<Message> message = V8_Ref_Get<Message>(self);
  Local<Value> str = message->Get();
  return V82RB(str);
}

VALUE v8_Message_GetSourceLine(VALUE self) {
  Local<Message> message = V8_Ref_Get<Message>(self);
  Local<Value> line = message->GetSourceLine();
  return V82RB(line);
}

VALUE v8_Message_GetScriptResourceName(VALUE self) {
  Local<Message> message = V8_Ref_Get<Message>(self);
  Handle<Value> name = message->GetScriptResourceName();
  return V82RB(name);
}

VALUE v8_Message_GetLineNumber(VALUE self) {
  Local<Message> message = V8_Ref_Get<Message>(self);
  return INT2FIX(message->GetLineNumber());
}

VALUE v8_Message_GetStartPosition(VALUE self) {
  Local<Message> message = V8_Ref_Get<Message>(self);
  return INT2FIX(message->GetStartPosition());
}

VALUE v8_Message_GetEndPosition(VALUE self) {
  Local<Message> message = V8_Ref_Get<Message>(self);
  return INT2FIX(message->GetEndPosition());
}

VALUE v8_Message_GetStartColumn(VALUE self) {
  Local<Message> message = V8_Ref_Get<Message>(self);
  return INT2FIX(message->GetStartColumn());
}

VALUE v8_Message_GetEndColumn(VALUE self) {
  Local<Message> message = V8_Ref_Get<Message>(self);
  return INT2FIX(message->GetEndColumn());
}
