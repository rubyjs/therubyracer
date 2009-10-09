#include "ruby.h"
#include "v8.h"
#include <stdio.h>

typedef struct v8_context {  
  v8_context() : context(v8::Context::New()) {}
  ~v8_context() {
    context.Dispose();
    printf("disposing of context\n");
  }
  v8::Persistent<v8::Context> context;
} v8_context;

extern "C" {
  void Init_v8();
}

VALUE v8_allocate(VALUE clazz);
void v8_mark(v8_context *s);
void v8_free(v8_context *s);

VALUE eval(VALUE self, VALUE javascript);

VALUE rb_mModule;
VALUE rb_cV8;

extern "C" {
  void Init_v8() {
    rb_mModule = rb_define_module("V8");
    rb_cV8 = rb_define_class_under(rb_mModule, "Context", rb_cObject);
    rb_define_alloc_func(rb_cV8, v8_allocate);
    rb_define_method(rb_cV8, "eval", (VALUE(*)(...)) eval, 1);
  }
}

VALUE v8_allocate(VALUE clazz) {
  printf("v8_allocate()\n");
  v8_context *s = new v8_context;
  return Data_Wrap_Struct(clazz, v8_mark, v8_free, s);
}

void v8_mark(v8_context *s) {
  // marked for gc?
}

void v8_free(v8_context *s) {
  delete s;
}

VALUE eval(VALUE self, VALUE javascript) {
  v8_context* s = 0;
  Data_Get_Struct(self, struct v8_context, s);
  const char* text = RSTRING(javascript)->ptr;
  
  v8::Context::Scope context_scope(s->context);
  v8::HandleScope handle_scope;
  v8::Handle<v8::String> source = v8::String::New(text);
  v8::Handle<v8::Script> script = v8::Script::Compile(source);
  v8::Handle<v8::Value> local_result = script->Run();
  printf("number value: %g\n", local_result->NumberValue());
  return Qnil;
}


// v8.c: In function ‘Init_v8’:
// v8.c:5: error: ‘rb_mV8’ undeclared (first use in this function)
// v8.c:5: error: (Each undeclared identifier is reported only once
// v8.c:5: error: for each function it appears in.)
// make: *** [v8.o] Error 1


// #include "glue.h"
// 
// VALUE rb_mV8;
// VALUE rb_cContext;
// 
// 
// VALUE rv8_Context__dealloc(VALUE self) {
//   printf("dealloc\n");
//   void * context;
//   Data_Get_Struct(self, void, context);
//   return Qnil;
// }
// 
// VALUE rv8_Context__alloc() {
//   printf("alloc\n");
//   void * context = cpp_v8_Context__new();
//   VALUE instance = Data_Wrap_Struct(rb_cContext, -1, rv8_Context__dealloc, context);
//   rb_obj_init(instance);
//   return instance;
// }
// 
// VALUE rv8_Context_pooh(VALUE self) {
//   int * i;
//   Data_Get_Struct(self, int , i);
//   return INT2NUM(*i);
// }
// 
// void Init_v8() {
//   rb_mV8 = rb_define_module("V8");
//   rb_cContext = rb_define_class_under(rb_mV8, "Context", rb_cObject);
//   rb_define_alloc_func(rb_cContext, rv8_Context__alloc);
// }

// #include <v8.h>
// #include <stdio.h>
// 
// 
// using namespace v8;
// 
// extern "C" {
//   #include "ruby.h"
// 
//   VALUE rv8;
//   VALUE rv8_Context;
// 
//   VALUE rv8_Context_new(...);
//   VALUE rv8_Context_enter(VALUE self);
//   
//   VALUE rv8_Context__alloc(VALUE klass);  
//   void rv8_Context_free(VALUE robj);
// 
//   void Init_rchassis() {
//     rv8 = rb_define_module("V8");    
//     rv8_Context = rb_define_class_under(rv8, "Context", rb_cObject);
//     rb_define_alloc_func(rv8_Context, rv8_Context__alloc);
//     rb_define_method(rv8_Context, "enter", rv8_Context_enter, 0);
//   }
//   
//   VALUE rv8_Context__alloc(VALUE klass) {
//     Persistent<Context> context = Context::New();
//     VALUE robj = Data_Wrap_Struct(rv8_Context, -1, rv8_Context_free, &context);
//     return robj;
//   }
//   
//   VALUE rv8_Context_enter(VALUE self) {
//     
//   }
//   
//   void rv8_Context_free(VALUE robj) {
//     Persistent<Context> * context;
//     printf("Let my people go!");
//     Data_Get_Struct(robj, Persistent<Context> , context);
//     printf("Get my people!");
//     context->Dispose();
//   }
//   
//   // VALUE rv8_Context_open(...) {
//   //   HandleScope scopes;
//   //   Persistent<Context> context = Context::New();
//   //   Context::Scope scope(context);
//   //   VALUE result = rb_yield(Qnil);
//   //   context.Dispose();
//   //   return result;
//   // }  
// }
