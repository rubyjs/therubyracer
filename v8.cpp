#include "ruby_data.h"
#include "generic_data.h"
#include "v8_data.h"
#include "v8_context.h"

#include <stdio.h>

extern "C" {
    /**
     * ruby init method for the extension
     */
    void Init_v8();
}

VALUE rb_mModule;
VALUE rb_cV8;

VALUE ruby_call_symbol;
VALUE ruby_respond_to_ID;

bool is_callable(VALUE& object) {
  return Qtrue == rb_funcall(object, ruby_respond_to_ID, 1, ruby_call_symbol);  
}

VALUE v8_what_is_this(VALUE self, VALUE object) {
  VALUE boolean;
  switch (TYPE(object)) {
    case T_NIL:		
    printf("nil\n");
    break;
    case T_OBJECT:	
    printf("ordinary object\n"); 
    if (responds_to(object, "call")) {
      printf("responds to call!<br/>");
    }
    break;
    case T_CLASS:		
      printf("class[%s]<br/>", rb_class2name(object)); 
      break;
    case T_MODULE:	printf("module\n"); break;
    case T_FLOAT:		printf("floating point number\n"); break;
    case T_STRING:	printf("string\n"); break;
    case T_REGEXP:	printf("regular expression\n"); break;
    case T_ARRAY:		printf("array\n"); break;
    case T_FIXNUM:	printf("Fixnum(31bit integer)\n"); break;
    case T_HASH:		printf("associative array\n"); break;
    case T_STRUCT:	printf("(Ruby) structure\n"); break;
    case T_BIGNUM:	printf("multi precision integer\n"); break;
    case T_FILE:		printf("IO\n"); break;
    case T_TRUE:		printf("true\n"); break;
    case T_FALSE:		printf("false\n"); break;
    case T_DATA:		
      printf("data... inspecting\n");
      if (responds_to(object, "call")) {
        printf("Responds to call!<br/>");
      } else {
        printf("Does *NOT* respond to call<br/>");
      }
      v8_what_is_this(Qnil, RDATA(object)->basic.klass);
      break;
    case T_SYMBOL:	printf("symbol\n"); break;

    default:
    printf("I have no idea!!!\n");
      rb_raise(rb_eTypeError, "not valid value");
      break;
  }
  
  return Qnil;
}

extern "C" {
    void Init_v8() {
      
        ruby_call_symbol = ID2SYM(rb_intern("call"));
        ruby_respond_to_ID = rb_intern("respond_to?");        
      
        rb_mModule = rb_define_module("V8");
        rb_cV8 = rb_define_class_under(rb_mModule, "Context", rb_cObject);
        rb_define_alloc_func(rb_cV8, v8_context_allocate);
        rb_define_method(rb_cV8, "eval", (VALUE(*)(...)) v8_context_eval, 1);
        
        rb_cV8_JSObject = rb_define_class_under(rb_mModule, "JSObject", rb_cObject);
        rb_define_alloc_func(rb_cV8_JSObject, v8_object_allocate);
        rb_define_method(rb_cV8_JSObject, "[]", (VALUE(*)(...)) v8_object_hash_access, 1);
        rb_define_method(rb_cV8_JSObject, "[]=", (VALUE(*)(...)) v8_object_hash_assignment, 2);
        rb_define_singleton_method(rb_mModule, "what_is_this?",(VALUE(*)(...)) v8_what_is_this, 1);
    }
}
