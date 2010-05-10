#include "v8_cxt.h"
#include "v8_str.h"
#include "v8_obj.h"
#include "v8_msg.h"
#include "v8_func.h"
#include "v8_script.h"
#include "v8_template.h"
#include "v8_standalone.h"
#include "converters.h"

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
        
        rb_mModule = rb_define_module("V8");
        rb_define_singleton_method(rb_mModule, "what_is_this?", (VALUE(*)(...)) v8_what_is_this, 1);
        
        V8_To = rb_define_module_under(rb_mModule, "To");
                
        //native module setup
        VALUE rb_mNative = rb_define_module_under(rb_mModule, "C");
        
        rr_init_cxt();
        rr_init_str();
        rr_init_script();
        rr_init_template();
        rr_init_obj();
        rr_init_func();
        
        
        V8_C_Message = rb_define_class_under(rb_mNative, "Message", rb_cObject);
        rb_define_method(V8_C_Message, "Get", (VALUE(*)(...))v8_Message_Get, 0);
        rb_define_method(V8_C_Message, "GetSourceLine", (VALUE(*)(...))v8_Message_GetSourceLine, 0);
        rb_define_method(V8_C_Message, "GetScriptResourceName", (VALUE(*)(...))v8_Message_GetScriptResourceName, 0);
        rb_define_method(V8_C_Message, "GetLineNumber", (VALUE(*)(...))v8_Message_GetLineNumber, 0);
        rb_define_method(V8_C_Message, "GetStartPosition", (VALUE(*)(...))v8_Message_GetStartPosition, 0);
        rb_define_method(V8_C_Message, "GetEndPosition", (VALUE(*)(...)) v8_Message_GetEndPosition, 0);
        rb_define_method(V8_C_Message, "GetStartColumn", (VALUE(*)(...)) v8_Message_GetStartColumn, 0);
        rb_define_method(V8_C_Message, "GetEndColumn", (VALUE(*)(...)) v8_Message_GetEndColumn, 0);
                
    }
}
