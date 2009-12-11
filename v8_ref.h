#ifndef _RUBY_V8_REF_
#define _RUBY_V8_REF_

#include <v8.h>
#include "ruby.h"

//the v8_ref wraps a v8 handle so that ruby can hold a reference to it.

struct v8_ref  {
  //takes a handle object and adds a new persistent handle for 
  //the referenced object
  v8_ref(v8::Handle<void> object, VALUE ref = 0);
  virtual ~v8_ref();
  v8::Persistent<void> handle;
  VALUE references;
};


//memory management
void v8_ref_mark(v8_ref* ref);
void v8_ref_free(v8_ref* ref);


//macros for helping make references

//creates a ruby VALUE reference out of any v8::Handle
// *clazz* a ruby VALUE corresponding to the ruby class of the resulting reference
// *handle* a v8::Handle<T> object to reference from ruby.
// example:
// VALUE myclass = rb_define_class("MyClass");
// VALUE instance = V8_Ref_Create(clazz, String::New("This is my instance"));

#define V8_Ref_Create(clazz,handle) Data_Wrap_Struct(clazz,v8_ref_mark, v8_ref_free, new v8_ref(handle))
#define V8_Ref_Create2(clazz,handle, references) Data_Wrap_Struct(clazz,v8_ref_mark, v8_ref_free, new v8_ref(handle, references))

//Dereferences a ruby reference to a V8 object and place it in a Local<T> handle.
// *type* V8 type of the reference. e.g. Context, Object, FunctionTemplate
// *var* the name of the variable to stuff the V8 object in.
// *value* the ruby VALUE object which contains the V8 reference (usually created with V8_Ref_Create())
#define V8_Ref_Get(type,var,value) v8_ref* __ ## var ## _ref__ = 0; Data_Get_Struct(value, struct v8_ref, __ ## var ## _ref__); Local<type> var = (type *)*__ ## var ## _ref__->handle;

#endif