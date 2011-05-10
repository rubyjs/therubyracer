#include "rr.h"
#include "v8_handle.h"
#include "v8_function.h"
#include "v8_template.h"
#include "v8_external.h"
#include "v8_callbacks.h"

using namespace v8;

namespace {

  VALUE ObjectTemplateClass;
  VALUE FunctionTemplateClass;

  struct v8_callback_data {
    VALUE handler;
    VALUE getter;
    VALUE setter;
    VALUE query;
    VALUE deleter;
    VALUE enumerator;
    VALUE data;
  };

  void delete_v8_data(Persistent<Value> value, void* parameter) {
    value.Dispose();
    delete (v8_callback_data*)parameter;
  }

  Local<External> make_v8_data(int argc, VALUE *argv, const char* argf) {
    VALUE handler; VALUE data;
    rb_scan_args(argc, argv, argf, &handler, &data);
    v8_callback_data* v8_data = new v8_callback_data();
    v8_data->handler = handler;
    v8_data->data = data;
    Local<External> external = External::New((void*)v8_data);
    Persistent<External>::New(external).MakeWeak((void*)v8_data, delete_v8_data);
    return external;
  }

  Persistent<Template> tmpl(VALUE self) {
    return rr_v8_handle<Template>(self);
  }
  Persistent<ObjectTemplate> obj(VALUE self) {
    return rr_v8_handle<ObjectTemplate>(self);
  }
  Persistent<FunctionTemplate> func(VALUE self) {
    return rr_v8_handle<FunctionTemplate>(self);
  }

  VALUE Set(VALUE self, VALUE name, VALUE value) {
    HandleScope handles;
    Local<String> key = rr_rb2v8(name)->ToString();
    Persistent<Data> data = rr_v8_handle<Data>(value);
    tmpl(self)->Set(key, data);
    return Qnil;
  }

  Handle<Value> RubyInvocationCallback(const Arguments& args) {
    Handle<External> v8_data_wrapper = Handle<External>::Cast(args.Data());
    v8_callback_data* v8_data = (v8_callback_data*)v8_data_wrapper->Value();
    if (RTEST(v8_data->handler)) {
    VALUE rb_args = rr_v82rb(args);
      rb_iv_set(rb_args, "data", v8_data->data);
      VALUE result = rb_funcall(v8_data->handler, rb_intern("call"), 1, rb_args);
      return rr_rb2v8(result);
    } else {
      return Handle<Value>();
    }
  }

  namespace Obj {


    v8_callback_data* accessor_info_data(const AccessorInfo& info) {
      Handle<External> v8_data_wrapper = Handle<External>::Cast(info.Data());
      return (v8_callback_data*)v8_data_wrapper->Value();
    }

    VALUE accessor_info_rb(const AccessorInfo& info) {
      VALUE rb_data = accessor_info_data(info)->data;
      VALUE rb_info = rr_v82rb(info);
      rb_iv_set(rb_info, "data", rb_data);
      return rb_info;
    }

    Local<External> accessor_info_data(VALUE getter, VALUE setter, VALUE query, VALUE deleter, VALUE enumerator, VALUE data) {
      v8_callback_data* v8_data = new v8_callback_data();
      v8_data->getter = getter;
      v8_data->setter = setter;
      v8_data->query = query;
      v8_data->deleter = deleter;
      v8_data->enumerator = enumerator;
      v8_data->data = data;
      Local<External> external = External::New((void*)v8_data);
      Persistent<External>::New(external).MakeWeak((void*)v8_data, delete_v8_data);
      return external;
    }

    /**
     * NamedProperty[Getter|Setter] are used as interceptors on object.
     * See ObjectTemplate::SetNamedPropertyHandler.
     */
    Handle<Value> RubyNamedPropertyGetter(Local<String> property, const AccessorInfo& info) {
      VALUE getter = accessor_info_data(info)->getter;
      return rr_rb2v8(rb_funcall(getter, rb_intern("call"), 2, rr_v82rb(property), accessor_info_rb(info)));
    }

    /**
     * Returns the value if the setter intercepts the request.
     * Otherwise, returns an empty handle.
     */
    Handle<Value> RubyNamedPropertySetter(Local<String> property, Local<Value> value, const AccessorInfo& info) {
      VALUE setter = accessor_info_data(info)->setter;
      VALUE result = rb_funcall(setter, rb_intern("call"), 3, rr_v82rb(property), rr_v82rb(value), accessor_info_rb(info));
      return rr_rb2v8(result);
    }


    /**
     * Returns a non-empty handle if the interceptor intercepts the request.
     * The result is true if the property exists and false otherwise.
     */
    Handle<Integer> RubyNamedPropertyQuery(Local<String> property, const AccessorInfo& info) {
      VALUE query = accessor_info_data(info)->query;
      VALUE result = rb_funcall(query, rb_intern("call"), 2, rr_v82rb(property), accessor_info_rb(info));
      Handle<Value> intercepts = rr_rb2v8(result);
      return intercepts.IsEmpty() ? Handle<Integer>() : Integer::New(None);
    }

    /**
     * Returns a non-empty handle if the deleter intercepts the request.
     * The return value is true if the property could be deleted and false
     * otherwise.
     */
    Handle<Boolean> RubyNamedPropertyDeleter(Local<String> property, const AccessorInfo& info) {
      VALUE deleter = accessor_info_data(info)->deleter;
      VALUE result = rb_funcall(deleter, rb_intern("call"), 2, rr_v82rb(property), accessor_info_rb(info));
      Handle<Value> intercepts = rr_rb2v8(result);
      return intercepts.IsEmpty() ? Handle<Boolean>() : intercepts->ToBoolean();
    }

    /**
     * Returns an array containing the names of the properties the named
     * property getter intercepts.
     */
    Handle<Array> RubyNamedPropertyEnumerator(const AccessorInfo& info) {
      VALUE enumerator = accessor_info_data(info)->enumerator;
      VALUE result = rb_funcall(enumerator, rb_intern("call"), 1, accessor_info_rb(info));
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

    /**
     * Returns the value of the property if the getter intercepts the
     * request.  Otherwise, returns an empty handle.
     */
    Handle<Value> RubyIndexedPropertyGetter(uint32_t index, const AccessorInfo& info) {
      VALUE getter = accessor_info_data(info)->getter;
      VALUE result = rb_funcall(getter, rb_intern("call"), 2, UINT2NUM(index), accessor_info_rb(info));
      return rr_rb2v8(result);
    }

    /**
     * Returns the value if the setter intercepts the request.
     * Otherwise, returns an empty handle.
     */
    Handle<Value> RubyIndexedPropertySetter(uint32_t index, Local<Value> value, const AccessorInfo& info) {
      VALUE setter = accessor_info_data(info)->setter;
      VALUE result = rb_funcall(setter, rb_intern("call"), 3, UINT2NUM(index), rr_v82rb(value), accessor_info_rb(info));
      return rr_rb2v8(result);
    }

    /**
     * Returns a non-empty handle if the interceptor intercepts the request.
     * The result is true if the property exists and false otherwise.
     */
     Handle<Integer> RubyIndexedPropertyQuery(uint32_t index, const AccessorInfo& info) {
       VALUE query = accessor_info_data(info)->query;
       VALUE result = rb_funcall(query, rb_intern("call"), 2, UINT2NUM(index), accessor_info_rb(info));
       Handle<Value> intercepts = rr_rb2v8(result);
       return intercepts.IsEmpty() ? Handle<Integer>() : Integer::New(None);
     }

    /**
     * Returns a non-empty handle if the deleter intercepts the request.
     * The return value is true if the property could be deleted and false
     * otherwise.
     */
     Handle<Boolean> RubyIndexedPropertyDeleter(uint32_t index, const AccessorInfo& info) {
       VALUE deleter = accessor_info_data(info)->deleter;
       VALUE result = rb_funcall(deleter, rb_intern("call"), 2, UINT2NUM(index), accessor_info_rb(info));
       Handle<Value> intercepts = rr_rb2v8(result);
       return intercepts.IsEmpty() ? Handle<Boolean>() : intercepts->ToBoolean();
     }

    /**
     * Returns an array containing the indices of the properties the
     * indexed property getter intercepts.
     */
     Handle<Array> RubyIndexedPropertyEnumerator(const AccessorInfo& info) {
       VALUE enumerator = accessor_info_data(info)->enumerator;
       VALUE result = rb_funcall(enumerator, rb_intern("call"), 1, accessor_info_rb(info));
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

    VALUE New(VALUE rbclass) {
      HandleScope handles;
      return rr_v8_handle_new(rbclass, ObjectTemplate::New());
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
    VALUE SetNamedPropertyHandler(VALUE self, VALUE getter, VALUE setter, VALUE query, VALUE deleter, VALUE enumerator, VALUE data) {
      HandleScope handles;
      if (!RTEST(getter)) {
        rb_raise(rb_eArgError, "you must supply at least a getter to V8::C::ObjectTemplate#SetNamedPropertyHandler()");
        return Qnil;
      }
      obj(self)->SetNamedPropertyHandler(
        RubyNamedPropertyGetter,
        RTEST(setter) ? RubyNamedPropertySetter : 0,
        RTEST(query) ? RubyNamedPropertyQuery : 0,
        RTEST(deleter) ? RubyNamedPropertyDeleter : 0,
        RTEST(enumerator) ? RubyNamedPropertyEnumerator : 0,
        accessor_info_data(getter, setter, query, deleter, enumerator, data)
      );
      return Qnil;
    }

    VALUE SetIndexedPropertyHandler(VALUE self, VALUE getter, VALUE setter, VALUE query, VALUE deleter, VALUE enumerator, VALUE data) {
      HandleScope scope;
      if (!RTEST(getter)) {
        rb_raise(rb_eArgError, "you must supply at least a getter to V8::C::ObjectTemplate#SetNamedPropertyHandler()");
        return Qnil;
      }
      obj(self)->SetIndexedPropertyHandler(
        RubyIndexedPropertyGetter,
        RTEST(setter) ? RubyIndexedPropertySetter : 0,
        RTEST(query) ? RubyIndexedPropertyQuery : 0,
        RTEST(deleter) ? RubyIndexedPropertyDeleter : 0,
        RTEST(enumerator) ? RubyIndexedPropertyEnumerator : 0,
        accessor_info_data(getter, setter, query, deleter, enumerator, data)
      );
      return Qnil;
    }
    VALUE SetCallAsFunctionHandler(int argc, VALUE *argv, VALUE self) {
      Handle<Value> v8_data = make_v8_data(argc, argv, "11");
      obj(self)->SetCallAsFunctionHandler(RubyInvocationCallback, v8_data);
      return Qnil;
    }
  }

  namespace Func {

    VALUE New(int argc, VALUE *argv, VALUE self) {
      HandleScope h;
      Handle<External> v8_data = make_v8_data(argc, argv, "02");
      Local<FunctionTemplate> t = FunctionTemplate::New(RubyInvocationCallback, v8_data);
      VALUE handle = rr_v8_handle_new(self,t);
      return handle;
    }
    VALUE SetCallHandler(int argc, VALUE *argv, VALUE self) {
      HandleScope h;
      Handle<Value> v8_data = make_v8_data(argc, argv, "11");
      func(self)->SetCallHandler(RubyInvocationCallback, v8_data);
      return Qnil;
    }
    VALUE PrototypeTemplate(VALUE self) {
      HandleScope scope;
      return rr_v8_handle_new(ObjectTemplateClass, func(self)->PrototypeTemplate());
    }
    VALUE InstanceTemplate(VALUE self) {
      HandleScope scope;
      return rr_v8_handle_new(ObjectTemplateClass, func(self)->InstanceTemplate());
    }
    VALUE Inherit(VALUE self, VALUE function_template) {
      HandleScope scope;
      func(self)->Inherit(func(function_template));
      return Qnil;
    }
    VALUE SetClassName(VALUE self, VALUE name) {
      HandleScope scope;
      func(self)->SetClassName(rr_rb2v8(name)->ToString());
      return Qnil;
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
  VALUE Template = rr_define_class("Template", rr_v8_handle_class());
  rr_define_method(Template, "Set", Set, 2);

  ObjectTemplateClass = rr_define_class("ObjectTemplate", Template);
  rr_define_singleton_method(ObjectTemplateClass, "New", Obj::New, 0);
  rr_define_method(ObjectTemplateClass, "NewInstance", Obj::NewInstance, 0);
  rr_define_method(ObjectTemplateClass, "SetNamedPropertyHandler", Obj::SetNamedPropertyHandler, 6);
  rr_define_method(ObjectTemplateClass, "SetIndexedPropertyHandler", Obj::SetIndexedPropertyHandler, 6);
  rr_define_method(ObjectTemplateClass, "SetCallAsFunctionHandler", Obj::SetCallAsFunctionHandler, -1);

  FunctionTemplateClass = rr_define_class("FunctionTemplate", Template);
  rr_define_singleton_method(FunctionTemplateClass, "New", Func::New, -1);
  rr_define_method(FunctionTemplateClass, "SetCallHandler", Func::SetCallHandler, -1);
  rr_define_method(FunctionTemplateClass, "PrototypeTemplate", Func::PrototypeTemplate, 0);
  rr_define_method(FunctionTemplateClass, "InstanceTemplate", Func::InstanceTemplate, 0);
  rr_define_method(FunctionTemplateClass, "Inherit", Func::Inherit, 1);
  rr_define_method(FunctionTemplateClass, "SetClassName", Func::SetClassName, 1);
  rr_define_method(FunctionTemplateClass, "GetFunction", Func::GetFunction, 0);
}
