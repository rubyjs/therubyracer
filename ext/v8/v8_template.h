#ifndef _RUBY_V8_TEMPLATE_
#define _RUBY_V8_TEMPLATE_ 

v8::Local<v8::ObjectTemplate> Racer_Create_V8_ObjectTemplate(VALUE object);

VALUE v8_Template_Set(VALUE self, VALUE name, VALUE value);

VALUE v8_ObjectTemplate_New(VALUE clazz);

VALUE v8_FunctionTemplate_New(int argc, VALUE *argv, VALUE self);
VALUE v8_FunctionTemplate_GetFunction(VALUE self);

#endif