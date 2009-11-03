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

extern "C" {
    void Init_v8() {
        rb_mModule = rb_define_module("V8");
        rb_cV8 = rb_define_class_under(rb_mModule, "Context", rb_cObject);
        rb_define_alloc_func(rb_cV8, v8_context_allocate);
        rb_define_method(rb_cV8, "eval", (VALUE(*)(...)) v8_context_eval, 1);
        
        rb_cV8_JSObject = rb_define_class_under(rb_mModule, "JSObject", rb_cObject);
        rb_define_alloc_func(rb_cV8_JSObject, v8_object_allocate);
        rb_define_method(rb_cV8_JSObject, "[]", (VALUE(*)(...)) v8_object_hash_access, 1);
        rb_define_method(rb_cV8_JSObject, "[]=", (VALUE(*)(...)) v8_object_hash_assignment, 2);
    }
}
