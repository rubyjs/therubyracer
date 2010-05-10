#include "v8_cxt.h"
#include "v8_str.h"
#include "v8_obj.h"
#include "v8_msg.h"
#include "v8_func.h"
#include "v8_script.h"
#include "v8_template.h"
#include "converters.h"

#include <stdio.h>

extern "C" {
    void Init_v8();
}

VALUE rb_mModule;

extern "C" {
    void Init_v8() {        
        V8_To = rb_define_module_under(rb_define_module("V8"), "To");
        rr_init_cxt();
        rr_init_str();
        rr_init_script();
        rr_init_template();
        rr_init_obj();
        rr_init_func();
        rr_init_msg();                                
    }
}
