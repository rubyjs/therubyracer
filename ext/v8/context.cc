#include "rr.h"

namespace rr {

void Context::Init() {
  ClassBuilder("Context").
    defineSingletonMethod("New", &New).
    defineSingletonMethod("GetCurrent", &GetCurrent).
    defineSingletonMethod("GetEntered", &GetEntered).
    defineSingletonMethod("GetCalling", &GetCalling).
    defineSingletonMethod("InContext", &InContext).
    defineMethod("Dispose", &Dispose).
    defineMethod("Clone", &Clone).
    defineMethod("Global", &Global).
    defineMethod("DetachGlobal", &Global).
    defineMethod("ReattachGlobal", &ReattachGlobal).
    defineMethod("SetSecurityToken", &SetSecurityToken).
    defineMethod("UseDefaultSecurityToken", &UseDefaultSecurityToken).
    defineMethod("GetSecurityToken", &GetSecurityToken).
    defineMethod("HasOutOfMemoryException", &HasOutOfMemoryException).
    defineMethod("SetEmbedderData", &SetEmbedderData).
    defineMethod("GetEmbedderData", &GetEmbedderData).
    defineMethod("AllowCodeGenerationFromStrings", &AllowCodeGenerationFromStrings).
    defineMethod("IsCodeGenerationFromStringsAllowed", &IsCodeGenerationFromStringsAllowed).
    defineMethod("Enter", &Enter).
    defineMethod("Exit", &Exit).
    store(&Class);
  ClassBuilder("ExtensionConfiguration").
    defineSingletonMethod("new", &ExtensionConfiguration::initialize).
    store(&ExtensionConfiguration::Class);
}

VALUE Context::Dispose(VALUE self) {
  Void(Context(self).dispose())
}

VALUE Context::Clone(VALUE self, VALUE new_rr_context) {
  // acquire the lock
  v8::Locker locker;
  v8::HandleScope handle_scope;

  // unwrap V8 contexts
  v8::Context* original_context = *((v8::Handle<v8::Context>) Context(self));
  v8::Context* new_context = *((v8::Handle<v8::Context>) Context(new_rr_context));

  // get the original global context
  v8::Local<v8::Value> key, value;

  original_context->Enter();

  v8::Local<v8::Object> original_global(original_context->Global());
  v8::Local<v8::Array> property_names = original_global->GetPropertyNames();
  uint32_t i, length = property_names->Length();
  std::vector< v8::Local<v8::Value> > values;

  for (i = 0; i < length; i++) {
    key = property_names->Get(i);
    value = original_global->Get(key);
    values.push_back(value);
  }

  original_context->Exit();

  // and copy everything to the new context
  new_context->Enter();

  v8::Local<v8::Object> new_global(new_context->Global());
  std::vector< v8::Local<v8::Value> >::iterator it;

  for (i = 0, it = values.begin(); i < length; i++, it++) {
    key = property_names->Get(i);
    value = *it;
    new_global->Set(key, value);
  }

  new_context->Exit();

  // unlock
  v8::Unlocker unlocker;

  return Qnil;
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

VALUE Context::SetEmbedderData(VALUE self, VALUE index, VALUE data) {
  Void(Context(self)->SetEmbedderData(NUM2INT(index), Value(data)));
}

VALUE Context::GetEmbedderData(VALUE self, VALUE index) {
  Void(Context(self)->GetEmbedderData(NUM2INT(index)));
}

VALUE Context::AllowCodeGenerationFromStrings(VALUE self, VALUE allow) {
  Void(Context(self)->AllowCodeGenerationFromStrings(RTEST(allow)));
}

VALUE Context::IsCodeGenerationFromStringsAllowed(VALUE self) {
  return Bool(Context(self)->IsCodeGenerationFromStringsAllowed());
}

VALUE ExtensionConfiguration::initialize(VALUE self, VALUE names) {
  int length = RARRAY_LENINT(names);
  const char* array[length];
  for (int i = 0; i < length; i++) {
    array[i] = RSTRING_PTR(rb_ary_entry(names, i));
  }
  return ExtensionConfiguration(new v8::ExtensionConfiguration(length, array));
}

VALUE Context::New(int argc, VALUE argv[], VALUE self) {
  VALUE extension_configuration; VALUE global_template; VALUE global_object;
  rb_scan_args(argc, argv, "03", &extension_configuration, &global_template, &global_object);
  v8::Persistent<v8::Context> context(v8::Context::New(
    ExtensionConfiguration(extension_configuration),
    *ObjectTemplate(global_template),
    *Object(global_object)
  ));
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

template <> void Pointer<v8::ExtensionConfiguration>::unwrap(VALUE value) {
  Data_Get_Struct(value, class v8::ExtensionConfiguration, pointer);
}

}
