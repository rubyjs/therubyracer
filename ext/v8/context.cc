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
  return Convert(Context(self)->Global());
}

VALUE Context::DetachGlobal(VALUE self) {
  Context(self)->DetachGlobal();
  return Qnil;
}

VALUE Context::ReattachGlobal(VALUE self, VALUE global) {
  Context(self)->ReattachGlobal(Object(global));
  return Qnil;
}

VALUE Context::GetEntered(VALUE self) {
  return Context::create(v8::Context::GetEntered());
}

VALUE Context::GetCurrent(VALUE self) {
  return Context::create(v8::Context::GetCurrent());
}

VALUE Context::GetCalling(VALUE self) {
  return Context::create(v8::Context::GetCalling());
}

VALUE Context::SetSecurityToken(VALUE self, VALUE token) {
  Context(self)->SetSecurityToken(Value(token));
  return Qnil;
}

VALUE Context::UseDefaultSecurityToken(VALUE self) {
  Context(self)->UseDefaultSecurityToken();
  return Qnil;
}

VALUE Context::GetSecurityToken(VALUE self) {
  return Convert(Context(self)->GetSecurityToken());
}

VALUE Context::HasOutOfMemoryException(VALUE self) {
  return Convert(Context(self)->HasOutOfMemoryException());
}

VALUE Context::InContext(VALUE self) {
  return Convert(v8::Context::InContext());
}

VALUE Context::SetData(VALUE self, VALUE data) {
  Context(self)->SetData(String(data));
  return Qnil;
}

VALUE Context::GetData(VALUE self) {
  return Convert(Context(self)->GetData());
}

VALUE Context::AllowCodeGenerationFromStrings(VALUE self, VALUE allow) {
  Context(self)->AllowCodeGenerationFromStrings(RTEST(allow));
  return Qnil;
}

VALUE Context::IsCodeGenerationFromStringsAllowed(VALUE self) {
  return Convert(Context(self)->IsCodeGenerationFromStringsAllowed());
}

VALUE Context::New(VALUE ContextClass) {
  v8::Persistent<v8::Context> context = v8::Context::New();
  Ref<v8::Context> ref = Context::create(context);
  context.Dispose();
  return ref;
}

VALUE Context::Enter(VALUE self) {
  Context(self)->Enter();
  return Qnil;
}

VALUE Context::Exit(VALUE self) {
  Context(self)->Exit();
  return Qnil;
}

}
