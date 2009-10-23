#ifndef __RUBY_V8_OBJECT__
#define __RUBY_V8_OBJECT__

#include <ruby.h>
#include <v8.h>

extern VALUE rb_cV8_JSObject;

typedef struct v8_object {
  v8_object();
  v8_object(v8::Handle<v8::Object>& object);
  ~v8_object();
  
  v8::Persistent<v8::Object> handle;
  bool dispose;
} v8_object;


VALUE v8_object_hash_access(VALUE self, VALUE key);

//memory management
VALUE v8_object_allocate(VALUE clazz);
void v8_object_mark(v8_object *object);
void v8_object_free(v8_object *object);


#endif