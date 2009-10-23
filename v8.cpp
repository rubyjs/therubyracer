#include "ruby_data.h"
#include "v8_data.h"
#include "generic_data.h"
#include <stdio.h>

/** 
 * struct that encapsulates a v8 context. this 
 * object's lifetime matches the lifetime of 
 * the ruby v8 context item.
 */
typedef struct v8_context {
    v8_context() : context(v8::Context::New()) {}
    ~v8_context() {
        context.Dispose();
    }
    v8::Persistent<v8::Context> context;
} v8_context;

extern "C" {
    /**
     * ruby init method for the extension
     */
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
        
        rb_cV8_JSObject = rb_define_class_under(rb_mModule, "JSObject", rb_cObject);
        rb_define_alloc_func(rb_cV8_JSObject, v8_object_allocate);
        rb_define_method(rb_cV8_JSObject, "[]", (VALUE(*)(...)) v8_object_hash_access, 1);
    }
}

VALUE v8_allocate(VALUE clazz) {
  v8_context *s = new v8_context;
  return Data_Wrap_Struct(clazz, v8_mark, v8_free, s);
}

void v8_mark(v8_context *s) {
  // marked for gc?
}

void v8_free(v8_context *s) {
  delete s;
}

/**
 * eval a javascript.  Currently, return
 * types can only be primitive types.
 * @param self ruby handle to a v8_context struct
 * @param javascript, should be a string 
 * @return the result of evaluating the script, if
 * it can be converted 
 */
VALUE eval(VALUE self, VALUE javascript) {
    v8_context* s = 0;
    Data_Get_Struct(self, struct v8_context, s);
    v8::Context::Scope context_scope(s->context);
    v8::HandleScope handle_scope;

    RubyValueSource<StringDest, std::string> tostring;
    const std::string text(tostring.push(javascript));
    v8::Handle<v8::String> source = v8::String::New(text.c_str());
    v8::Handle<v8::Script> script = v8::Script::Compile(source);

    v8::Handle<v8::Value> local_result = script->Run();
    V8HandleSource<RubyDest, VALUE> toValue;
    return toValue.push(local_result);
}
