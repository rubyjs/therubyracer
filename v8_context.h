#ifndef __RUBY_V8_CONTEXT__
#define __RUBY_V8_CONTEXT__

#include <ruby.h>
#include <v8.h>

typedef struct v8_context {
  v8_context();
  ~v8_context();

  v8::Handle<v8::Value> eval(const char* javascript);
    
  v8::Persistent<v8::Context> handle;  
  
  typedef struct ensure {
    inline ensure() {    
      if (!v8::Context::InContext()) {
        cxt = v8::Context::New();
        cxt->Enter();
      }
    }

    ~ensure() {
      if (!cxt.IsEmpty()) {
        cxt->Exit();
      }
      cxt.Dispose();
    }

    v8::Persistent<v8::Context> cxt;
    v8::HandleScope handles;
  } ensure;  
  
} v8_context;


//memory management
VALUE v8_context_allocate(VALUE clazz);
void v8_context_mark(v8_context *context);
void v8_context_free(v8_context *context);

//methods
VALUE v8_context_eval(VALUE self, VALUE javascript);

#endif