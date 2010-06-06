#include "rr.h"
#include "v8_ref.h"
#include "v8_func.h"
#include "v8_template.h"
#include "v8_external.h"
#include "v8_callbacks.h"

using namespace v8;

namespace {

  VALUE rb_hash_lookup(VALUE hash, const char *key) {
    return rb_funcall(hash, rb_intern("[]"), 1, rb_str_new2(key));
  }

  VALUE rb_hash_aset(VALUE hash, const char *key, VALUE value) {
    return ::rb_hash_aset(hash, rb_str_new2(key), value);
  }

  Local<Template> tmpl(VALUE self) {
    return V8_Ref_Get<Template>(self);
  }
  Local<ObjectTemplate> obj(VALUE self) {
    return V8_Ref_Get<ObjectTemplate>(self);
  }
  Local<FunctionTemplate> func(VALUE self) {
    return V8_Ref_Get<FunctionTemplate>(self);
  }

  VALUE Set(VALUE self, VALUE name, VALUE value) {
    HandleScope handles;
    Local<String> key = rr_rb2v8(name)->ToString();
    Local<Data> data = V8_Ref_Get<Data>(value);
    tmpl(self)->Set(key, data);
    return Qnil;
  }

  namespace Obj {

    /**
     * NamedProperty[Getter|Setter] are used as interceptors on object.
     * See ObjectTemplate::SetNamedPropertyHandler.
     */
    Handle<Value> RubyNamedPropertyGetter(Local<String> property, const AccessorInfo& info) {
      VALUE code = (VALUE)External::Unwrap(info.Data());
      VALUE getter = rb_hash_lookup(code, "getter");
      return rr_rb2v8(rb_funcall(getter, rb_intern("call"), 2, rr_v82rb(property), rr_v82rb(info)));
    }

    /**
     * Returns the value if the setter intercepts the request.
     * Otherwise, returns an empty handle.
     */
    Handle<Value> RubyNamedPropertySetter(Local<String> property, Local<Value> value, const AccessorInfo& info) {
      HandleScope handles;
      VALUE code = (VALUE)External::Unwrap(info.Data());
      VALUE setter = rb_hash_lookup(code, "setter");
      VALUE result = rb_funcall(setter, rb_intern("call"), 3, rr_v82rb(property), rr_v82rb(value), rr_v82rb(info));
      return rr_rb2v8(result);
    }


    /**
     * Returns a non-empty handle if the interceptor intercepts the request.
     * The result is true if the property exists and false otherwise.
     */
    Handle<Boolean> RubyNamedPropertyQuery(Local<String> property, const AccessorInfo& info) {
      HandleScope handles;
      VALUE code = (VALUE)External::Unwrap(info.Data());
      VALUE query = rb_hash_lookup(code, "query");
      VALUE result = rb_funcall(query, rb_intern("call"), 2, rr_v82rb(property), rr_v82rb(info));
      Handle<Value> intercepts = rr_rb2v8(result);
      return intercepts.IsEmpty() ? Handle<Boolean>() : intercepts->ToBoolean();
    }

    /**
     * Returns a non-empty handle if the deleter intercepts the request.
     * The return value is true if the property could be deleted and false
     * otherwise.
     */
    Handle<Boolean> RubyNamedPropertyDeleter(Local<String> property, const AccessorInfo& info) {
      HandleScope handles;
      VALUE code = (VALUE)External::Unwrap(info.Data());
      VALUE deleter = rb_hash_lookup(code, "deleter");
      VALUE result = rb_funcall(deleter, rb_intern("call"), 2, rr_v82rb(property), rr_v82rb(info));
      Handle<Value> intercepts = rr_rb2v8(result);
      return intercepts.IsEmpty() ? Handle<Boolean>() : intercepts->ToBoolean();
    }

    /**
     * Returns an array containing the names of the properties the named
     * property getter intercepts.
     */
    Handle<Array> RubyNamedPropertyEnumerator(const AccessorInfo& info) {
      HandleScope handles;
      VALUE code = (VALUE)External::Unwrap(info.Data());
      VALUE enumerator = rb_hash_lookup(code, "enumerator");
      VALUE result = rb_funcall(enumerator, rb_intern("call"), 1, rr_v82rb(info));
      Handle<Value> v(rr_rb2v8(result));
      if (v.IsEmpty()) {
        return Array::New();
      } else if (!v->IsArray()) {
        Local<Array> a = Array::New();
        a->Set(Integer::New(0), v->ToString());
        return a;
      } else {
        return (Handle<Array>)Array::Cast(*v);
      }
    }

    VALUE New(VALUE clazz) {
      HandleScope handles;
      return V8_Ref_Create(clazz, ObjectTemplate::New());
    }
    VALUE NewInstance(VALUE self) {
      HandleScope scope;
      if (!Context::InContext()) {
        rb_raise(rb_eScriptError, "ObjectTemplate::NewInstance() called without an entered Context");
        return Qnil;
      }
      Local<Object> object(obj(self)->NewInstance());
      if (object.IsEmpty()) {
        rb_raise(rb_eFatal, "V8 returned empty handle on call to ObjectTemplate::NewInstance()");
        return Qnil;
      }
      return rr_v82rb(object);
    }
    VALUE SetNamedPropertyHandler(VALUE self, VALUE getter, VALUE setter, VALUE query, VALUE deleter, VALUE enumerator) {
      HandleScope handles;
      if (!RTEST(getter)) {
        rb_raise(rb_eArgError, "you must supply at least a getter to V8::C::ObjectTemplate#SetNamedPropertyHandler()");
        return Qnil;
      }
      VALUE data = rb_hash_new();
      rb_hash_aset(data, "getter", getter);
      rb_hash_aset(data, "setter", setter);
      rb_hash_aset(data, "query", query);
      rb_hash_aset(data, "deleter", deleter);
      rb_hash_aset(data, "enumerator", enumerator);
      rr_v8_ref_setref(self, "data", data);
      obj(self)->SetNamedPropertyHandler(
        RubyNamedPropertyGetter,
        RTEST(setter) ? RubyNamedPropertySetter : 0,
        RTEST(query) ? RubyNamedPropertyQuery : 0,
        RTEST(deleter) ? RubyNamedPropertyDeleter : 0,
        RTEST(enumerator) ? RubyNamedPropertyEnumerator : 0,
        rr_v8_external_create(data)
      );
      return Qnil;
    }
  }

  namespace Func {

    Handle<Value> RubyInvocationCallback(const Arguments& args) {
      HandleScope handles;
      VALUE code = (VALUE)External::Unwrap(args.Data());
      VALUE rb_args = rr_v82rb(args);
      VALUE result = rb_funcall(code, rb_intern("call"), 1, rb_args);
      return rr_rb2v8(result);
    }

    VALUE New(VALUE function_template) {
      HandleScope handles;
      rb_need_block();
      VALUE code = rb_block_proc();
      if (NIL_P(code)) {
        return Qnil;
      }
      Local<FunctionTemplate> templ = FunctionTemplate::New(RubyInvocationCallback, rr_v8_external_create(code));
      return V8_Ref_Create(function_template,templ,code);
    }
    VALUE GetFunction(VALUE self) {
      HandleScope handles;
      if (!Context::InContext()) {
        rb_raise(rb_eScriptError, "calls to FunctionTemplate::GetFunction() require a Context to be entered");
        return Qnil;
      }
      return rr_v82rb(func(self)->GetFunction());
    }
  }
}

void rr_init_template() {
  VALUE Template = rr_define_class("Template");
  rr_define_method(Template, "Set", Set, 2);

  VALUE ObjectTemplate = rr_define_class("ObjectTemplate", Template);
  rr_define_singleton_method(ObjectTemplate, "New", Obj::New, 0);
  rr_define_method(ObjectTemplate, "NewInstance", Obj::NewInstance, 0);
  rr_define_method(ObjectTemplate, "SetNamedPropertyHandler", Obj::SetNamedPropertyHandler, 5);

  VALUE FunctionTemplate = rr_define_class("FunctionTemplate", Template);
  rr_define_singleton_method(FunctionTemplate, "New", Func::New, 0);
  rr_define_method(FunctionTemplate, "GetFunction", Func::GetFunction, 0);


}

Handle<ObjectTemplate> rr_template_std_rubyobject() {
  VALUE V8 = rb_define_module("V8");
  VALUE to = rb_define_module_under(V8, "To");
  VALUE tmpl = rb_funcall(to, rb_intern("template"), 0);
  return V8_Ref_Get<ObjectTemplate>(tmpl);
}
