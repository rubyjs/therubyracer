#include "v8_standalone.h"

VALUE ruby_call_symbol;
VALUE ruby_respond_to_ID;

VALUE ruby_proc_class;
VALUE ruby_method_class;


bool is_callable(VALUE& object) {
  return Qtrue == rb_funcall(object, ruby_respond_to_ID, 1, ruby_call_symbol);  
}

bool is_function(VALUE& object) {
  VALUE klass = RDATA(object)->basic.klass;
  return  klass == ruby_proc_class || klass == ruby_method_class;
}

/**
 * Debugging aid
 */
VALUE v8_what_is_this(VALUE self, VALUE object) {
  switch (TYPE(object)) {
    case T_NIL:		
    printf("nil\n");
    break;
    case T_OBJECT:	
    printf("ordinary object\n"); 
    if (is_callable(object)) {
      printf("responds to call!<br/>");
    }
    break;
    case T_CLASS:		
      printf("class[%s]<br/>", rb_class2name(object)); 
      break;
    case T_MODULE:	printf("module\n"); break;
    case T_FLOAT:	printf("floating point number\n"); break;
    case T_STRING:	printf("string\n"); break;
    case T_REGEXP:	printf("regular expression\n"); break;
    case T_ARRAY:	printf("array\n"); break;
    case T_FIXNUM:	printf("Fixnum(31bit integer)\n"); break;
    case T_HASH:	printf("associative array\n"); break;
    case T_STRUCT:	printf("(Ruby) structure\n"); break;
    case T_BIGNUM:	printf("multi precision integer\n"); break;
    case T_FILE:	printf("IO\n"); break;
    case T_TRUE:	printf("true\n"); break;
    case T_FALSE:	printf("false\n"); break;
    case T_DATA:	
      printf("data... inspecting\n");
      if (is_function(object)) {
        printf("It's a function!!!<br/>");
      }
      if (is_callable(object)) {
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
