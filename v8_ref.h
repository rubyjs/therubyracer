#ifndef _RUBY_V8_REF_
#define _RUBY_V8_REF_

#include <v8.h>


//the v8_ref wraps a v8 handle so that ruby can hold a reference to it.

struct v8_ref  {
  //takes a handle object and adds a new persistent handle for 
  //the referenced object
  v8_ref(v8::Handle<void> object);
  virtual ~v8_ref();
  v8::Persistent<void> handle;
};


//memory management
void v8_ref_mark(v8_ref* ref);
void v8_ref_free(v8_ref* ref);


//macros for helping make references

#define V8_Ref_Create(clazz,handle) Data_Wrap_Struct(clazz,v8_ref_mark, v8_ref_free, new v8_ref(handle))

#define V8_Ref_Get(type,var,value) v8_ref* __ref__ = 0; Data_Get_Struct(value, struct v8_ref, __ref__); Local<type> var = (type *)*__ref__->handle;

#endif