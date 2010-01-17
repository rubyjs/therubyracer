#ifndef CALLBACKS_H_8VK3LWBG
#define CALLBACKS_H_8VK3LWBG

#include <v8.h>

v8::Handle<v8::Value> RacerRubyInvocationCallback(const v8::Arguments& args); 

v8::Handle<v8::Value> RacerRubyNamedPropertyGetter(v8::Local<v8::String> property, const v8::AccessorInfo& info);
v8::Handle<v8::Value> RacerRubyNamedPropertySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
v8::Handle<v8::Boolean> RacerRubyNamedPropertyQuery(v8::Local<v8::String> property, const v8::AccessorInfo& info);
v8::Handle<v8::Boolean> RacerRubyNamedPropertyDeleter(v8::Local<v8::String> property, const v8::AccessorInfo& info);
v8::Handle<v8::Array> RacerRubyNamedPropertyEnumerator(const v8::AccessorInfo& info);

#endif /* end of include guard: CALLBACKS_H_8VK3LWBG */
