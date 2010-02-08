#include "converters.h"
#include "v8_msg.h"
#include "v8_ref.h"

using namespace v8;

VALUE V8_C_Message;

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
