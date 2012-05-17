#include "rr.h"

namespace rr {

void Context::Init() {
  ClassBuilder("Context").
    defineSingletonMethod("New", &New).
    defineSingletonMethod("GetCurrent", &GetCurrent).
    defineSingletonMethod("GetEntered", &GetEntered).
    defineSingletonMethod("GetCalling", &GetCalling).
    defineSingletonMethod("InContext", &InContext).
    defineMethod("Global", &Global).
    defineMethod("DetachGlobal", &Global).
    defineMethod("ReattachGlobal", &ReattachGlobal).
    defineMethod("SetSecurityToken", &SetSecurityToken).
    defineMethod("UseDefaultSecurityToken", &UseDefaultSecurityToken).
    defineMethod("GetSecurityToken", &GetSecurityToken).
    defineMethod("HasOutOfMemoryException", &HasOutOfMemoryException).
    defineMethod("SetData", &SetData).
    defineMethod("GetData", &GetData).
    defineMethod("AllowCodeGenerationFromStrings", &AllowCodeGenerationFromStrings).
    defineMethod("IsCodeGenerationFromStringsAllowed", &IsCodeGenerationFromStringsAllowed).
    defineMethod("Enter", &Enter).
    defineMethod("Exit", &Exit).
    store(&Class);
}

VALUE Context::Global(VALUE self) {
  return Object(Context(self)->Global());
}

VALUE Context::DetachGlobal(VALUE self) {
  Void(Context(self)->DetachGlobal());
}

VALUE Context::ReattachGlobal(VALUE self, VALUE global) {
  Void(Context(self)->ReattachGlobal(Object(global)));
}

VALUE Context::GetEntered(VALUE self) {
  return Context(v8::Context::GetEntered());
}

VALUE Context::GetCurrent(VALUE self) {
  return Context(v8::Context::GetCurrent());
}

VALUE Context::GetCalling(VALUE self) {
  return Context(v8::Context::GetCalling());
}

VALUE Context::SetSecurityToken(VALUE self, VALUE token) {
  Void(Context(self)->SetSecurityToken(Value(token)));
}

VALUE Context::UseDefaultSecurityToken(VALUE self) {
  Void(Context(self)->UseDefaultSecurityToken());
}

VALUE Context::GetSecurityToken(VALUE self) {
  return Value(Context(self)->GetSecurityToken());
}

VALUE Context::HasOutOfMemoryException(VALUE self) {
  return Bool(Context(self)->HasOutOfMemoryException());
}

VALUE Context::InContext(VALUE self) {
  return Bool(v8::Context::InContext());
}

VALUE Context::SetData(VALUE self, VALUE data) {
  Void(Context(self)->SetData(String(data)));
}

VALUE Context::GetData(VALUE self) {
  return Value(Context(self)->GetData());
}

VALUE Context::AllowCodeGenerationFromStrings(VALUE self, VALUE allow) {
  Void(Context(self)->AllowCodeGenerationFromStrings(RTEST(allow)));
}

VALUE Context::IsCodeGenerationFromStringsAllowed(VALUE self) {
  return Bool(Context(self)->IsCodeGenerationFromStringsAllowed());
}

VALUE Context::New(VALUE ContextClass) {
  v8::Persistent<v8::Context> context = v8::Context::New();
  Context reference(context);
  context.Dispose();
  return reference;
}

VALUE Context::Enter(VALUE self) {
  Void(Context(self)->Enter());
}

VALUE Context::Exit(VALUE self) {
  Void(Context(self)->Exit());
}

}
