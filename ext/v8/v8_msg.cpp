
#include "v8_msg.h"
#include "v8_ref.h"

using namespace v8;

VALUE V8_C_Message;

VALUE V8_Wrap_Message(Handle<v8::Message> msg) {
  return V8_Ref_Create(V8_C_Message, msg);
}