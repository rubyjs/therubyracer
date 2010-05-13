#include "rr.h"
#include "v8_cxt.h"
#include "v8_msg.h"
#include "v8_template.h"
#include "converters.h"

using namespace v8;

VALUE V8_C_Context;

//TODO: rename everything to Context_
//TODO: do the object init from within here

void rr_init_cxt() {
  VALUE Context = V8_C_Context = rr_define_class("Context");
  rb_define_singleton_method(Context, "new", (VALUE(*)(...)) v8_Context_New, -1);
  rb_define_singleton_method(Context, "InContext", (VALUE(*)(...)) v8_Context_InContext, 0);
  rb_define_singleton_method(Context, "GetCurrent", (VALUE(*)(...)) v8_Context_GetCurrent, 0);
  rb_define_method(Context, "Global", (VALUE(*)(...)) v8_cxt_Global, 0);
  rb_define_method(Context, "open", (VALUE(*)(...)) v8_cxt_open, 0);
  rb_define_method(Context, "eval", (VALUE(*)(...)) v8_cxt_eval, 2);
  rb_define_method(Context, "eql?", (VALUE(*)(...)) v8_cxt_eql, 1);
  rb_define_method(Context, "==", (VALUE(*)(...)) v8_cxt_eql, 1);  
}

VALUE v8_Context_New(int argc, VALUE *argv, VALUE self) {
  HandleScope handles;
  VALUE scope;
  rb_scan_args(argc,argv, "01", &scope);
  if (NIL_P(scope)) {
    return V8_Ref_Create(self, Context::New());
  } else {
    Persistent<Context> context = Context::New(0, Racer_Create_V8_ObjectTemplate(scope));
    Context::Scope enter(context);
    context->Global()->SetHiddenValue(String::New("TheRubyRacer::RubyObject"), External::Wrap((void *)scope));
    VALUE ref = V8_Ref_Create(self, context, scope);
    context.Dispose();
    return ref;
  }
}

VALUE v8_Context_InContext(VALUE self) {
  return Context::InContext() ? Qtrue : Qfalse;  
}

VALUE v8_Context_GetCurrent(VALUE self) {
  HandleScope handles;
  if (Context::InContext()) {
    Local<Context> current = Context::GetCurrent();
    return V8_Ref_Create(self, current);    
  } else {
    return Qnil;
  }
}

VALUE v8_cxt_Global(VALUE self) {
  HandleScope handles;
  Local<Context> cxt = V8_Ref_Get<Context>(self);
  Local<Value> global = *cxt->Global();
  return V82RB(global);
}

VALUE v8_cxt_open(VALUE self) {
  HandleScope handles;
  Local<Context> cxt = V8_Ref_Get<Context>(self);
  Context::Scope enter(cxt);
  if (rb_block_given_p()) {
    return rb_yield(self);
  } else {
    return Qnil;
  }
}

VALUE Racer_Error_Message(TryCatch& exception) {
  VALUE msg = V8_Wrap_Message(exception.Message());
  Local<Value> stack = exception.StackTrace();
  if (!stack.IsEmpty()) {
    rb_iv_set(msg,"@stack",V82RB(stack));    
  }
  return msg;  
}

VALUE v8_cxt_eval(VALUE self, VALUE source, VALUE filename) {
  printf("In cxt eval()\n");
  HandleScope handles;
  TryCatch exceptions;
  Local<Context> cxt = V8_Ref_Get<Context>(self);
  Context::Scope enter(cxt);
  Local<Value> source_str = RB2V8(source);
  Local<Value> source_name = RTEST(filename) ? RB2V8(filename) : *String::New("<eval>");
  Local<Script> script = Script::Compile(source_str->ToString(), source_name);
	if (exceptions.HasCaught()) {	  
    return Racer_Error_Message(exceptions);
	} 
  Local<Value> result = script->Run();
  if (exceptions.HasCaught()) {
    return Racer_Error_Message(exceptions);
  } else {
    printf("about to convert result\n");
    return rr_v82rb(result);
    // return V82RB(result);
  }
}

VALUE v8_cxt_eql(VALUE self, VALUE other) {
  HandleScope handles;
  if (RTEST(CLASS_OF(other) != V8_C_Context)) {
    return Qnil;
  } else {
    Local<Context> cxt = V8_Ref_Get<Context>(self);
    Local<Context> that = V8_Ref_Get<Context>(other);
    return cxt == that ? Qtrue : Qfalse;
  }
  return Qnil;
}


