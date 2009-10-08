#include "ruby.h"
#include <stdio.h>

static VALUE nasty_hack;

VALUE evalJS_v8(VALUE self);

void Init_v8() {
  VALUE rb_mV8  = rb_define_module("V8");  
  nasty_hack = rb_mV8;
  VALUE rb_cV8_Context = rb_define_class_under(rb_mV8, "Context", rb_cObject);

  rb_define_method(rb_cV8_Context, "eval_js", evalJS_v8, 0);
		   //VALUE (*func)(), int argc)
  
  //rb_define_alloc_func(rb_cContext, rv8_Context__alloc);
}



VALUE evalJS_v8(VALUE self) {

  if (TYPE(self)== T_OBJECT) printf("OBJECT\n");

  ID classGetter = rb_intern("class");
    VALUE myClass = rb_funcall(self, classGetter, 0);

  printf("same?  %d\n", nasty_hack == self);

  return self;
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
