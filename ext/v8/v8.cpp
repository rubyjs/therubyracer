#include "v8_cxt.h"
#include "v8_str.h"
#include "v8_script.h"
#include "v8_template.h"
#include "v8_standalone.h"

#include <stdio.h>

extern "C" {
    /**
     * ruby init method for the extension
     */
    void Init_v8();
}

VALUE rb_mModule;
VALUE rb_cV8;

extern "C" {
    void Init_v8() {
      
        ruby_call_symbol = ID2SYM(rb_intern("call"));
        ruby_respond_to_ID = rb_intern("respond_to?");
        ruby_proc_class = rb_eval_string("::Proc");
        ruby_method_class = rb_eval_string("::Method");
      
        rb_mModule = rb_define_module("V8");
                
        //native module setup
        VALUE rb_mNative = rb_define_module_under(rb_mModule, "C");
        
        //native context
        VALUE V8__C__Context = rb_define_class_under(rb_mNative, "Context", rb_cObject);
        rb_define_singleton_method(V8__C__Context, "new", (VALUE(*)(...)) v8_Context_New, -1);
        rb_define_method(V8__C__Context, "Global", (VALUE(*)(...)) v8_cxt_Global, 0);
        rb_define_method(V8__C__Context, "open", (VALUE(*)(...)) v8_cxt_open, 0);
        
        //native String
        VALUE V8__C__String = rb_define_class_under(rb_mNative, "String", rb_cObject);
        rb_define_singleton_method(V8__C__String, "new", (VALUE(*)(...)) v8_str_new, 1);
        rb_define_method(V8__C__String, "to_s", (VALUE(*)(...)) v8_str_to_s, 0);
        
        VALUE V8__C__Script = rb_define_class_under(rb_mNative, "Script", rb_cObject);
        rb_define_singleton_method(V8__C__Script, "new", (VALUE(*)(...)) v8_script_new, 1);
        rb_define_method(V8__C__Script, "Run", (VALUE(*)(...)) v8_script_Run, 0);
        
        VALUE V8__C__Template = rb_define_class_under(rb_mNative, "Template", rb_cObject);
        rb_define_method(V8__C__Template, "Set", (VALUE(*)(...))v8_Template_Set, 2);
        
        VALUE V8__C__ObjectTemplate = rb_define_class_under(rb_mNative, "ObjectTemplate", V8__C__Template);
        rb_define_singleton_method(V8__C__ObjectTemplate, "new", (VALUE(*)(...))v8_ObjectTemplate_New, 0);
        
        VALUE V8__C__FunctionTemplate = rb_define_class_under(rb_mNative, "FunctionTemplate", V8__C__Template);
        rb_define_singleton_method(V8__C__FunctionTemplate, "new", (VALUE(*)(...))v8_FunctionTemplate_New, -1);                
    }
}
