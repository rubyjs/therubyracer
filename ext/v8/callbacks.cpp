#include <ruby.h>

#include "callbacks.h"
#include "converters.h"

using namespace v8;

namespace {
  VALUE unwrap(const AccessorInfo& info) {
    return (VALUE)External::Unwrap(info.Data());
  }
  
  //returns all the ruby methods that can be called from
  //this object.
  //1.8: public_methods() return strings
  //1.9: publi_methods() return symbols
  //convert them all into string

  VALUE CALLABLE_METHODS(VALUE object) {
    VALUE methods = rb_funcall(object, rb_intern("public_methods"), 1, Qfalse);
    int length = RARRAY_LEN(methods);
    VALUE str_methods = rb_ary_new2(length);
    for (int i = 0; i < length; i++) {
      VALUE method = rb_ary_entry(methods, i);
      rb_ary_store(str_methods,i, rb_obj_as_string(method));
    }
    return str_methods;
  }

  Local<Array> TO_ARRAY(Arguments& args) {
    Local<Array> array = Array::New(args.Length());
    for (int i = 0; i < args.Length(); i++) {
      array->Set(Integer::New(i), args[i]);
    }
    return array;
  }
  
  Local<Value> Racer_Call_Ruby_Method(VALUE object, VALUE method, Local<Array> args) {
    VALUE * arguments = new VALUE[args->Length()];
    for (unsigned int i = 0; i < args->Length(); i++) {
      Handle<Value> val = args->Get(Integer::New(i));
      arguments[i] = V82RB(val);
    }
    VALUE result = rb_funcall2(object, rb_to_id(method), args->Length(), arguments);
    Local<Value> converted = RB2V8(result);
    return converted;
  }
  
  Local<Value> Racer_Access_Ruby_Property(VALUE object, VALUE name) { 
    VALUE method = rb_funcall(object, rb_intern("method"), 1, name);
    if (FIX2INT(rb_funcall(method, rb_intern("arity"), 0)) == 0) {
      return Racer_Call_Ruby_Method(object, name, Array::New(0));      
    } else {
      return RB2V8(method);
    }        
  }
}

Handle<Value> RacerRubyInvocationCallback(const Arguments& args) {
  VALUE code = (VALUE)External::Unwrap(args.Data());
  if (NIL_P(code)) {
    return Null();
  } else {      
    VALUE* arguments = new VALUE[args.Length()];
    for(int c=0;c<args.Length(); ++c) {
      Handle<Value> val = args[c];
      arguments[c] = V82RB(val);
    }
      
    VALUE result = rb_funcall2(code, rb_intern("call"), args.Length(), arguments);
    delete [] arguments;
    
    Handle<Value> convertedResult = RB2V8(result);
    return convertedResult  ;
  }
}




/**
 * NamedProperty[Getter|Setter] are used as interceptors on object.
 * See ObjectTemplate::SetNamedPropertyHandler.
 */

Handle<Value> RacerRubyNamedPropertyGetter(Local<String> property, const AccessorInfo& info) {
  // printf("Getter '%s'<br/>", *String::AsciiValue(property));
  if (property->Length() == 0) {
    return Handle<Value>();
  }
  VALUE object = unwrap(info);
  VALUE camel_name = V82RB((Local<Value>&)property);
  VALUE perl_name = rb_funcall(V8_To, rb_intern("perl_case"), 1, camel_name);
  VALUE methods = CALLABLE_METHODS(object);
  
  if (RTEST(rb_ary_includes(methods, perl_name))) {
    return Racer_Access_Ruby_Property(object, perl_name);
  }
  if (RTEST(rb_ary_includes(methods, camel_name))) {
    return Racer_Access_Ruby_Property(object, camel_name);
  }
  return Handle<Value>();
}

/**
 * Returns the value if the setter intercepts the request.
 * Otherwise, returns an empty handle.
 */
Handle<Value> RacerRubyNamedPropertySetter(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  if (property->Length() == 0) {
    return Handle<Value>();
  }
  // printf("Setter: '%s'<br/>", *String::AsciiValue(property));
  std::string setter = V82String((Handle<Value>&)property);
  setter += "=";
  Local<String> setter_name = String::New(setter.c_str());
  VALUE object = unwrap(info);
  VALUE camel_name = V82RB((Local<Value>&)setter_name);
  VALUE perl_name = rb_funcall(V8_To, rb_intern("perl_case"), 1, camel_name);
  VALUE methods = CALLABLE_METHODS(object);
  Local<Array> args = Array::New(1);
  args->Set(Integer::New(0), value);
  if (RTEST(rb_ary_includes(methods, perl_name))) {    
    Racer_Call_Ruby_Method(object, perl_name, args);
    return value;
  }
  if (RTEST(rb_ary_includes(methods, camel_name))) {
    Racer_Call_Ruby_Method(object, camel_name, args);
    return value;
  }    
  return Handle<Value>();
}

/**
 * Returns a non-empty handle if the interceptor intercepts the request.
 * The result is true if the property exists and false otherwise.
 */
Handle<Boolean> RacerRubyNamedPropertyQuery(Local<String> property, const AccessorInfo& info) {
  // printf("Query: '%s'<br/>", *String::AsciiValue(property));
  if (property->Length() == 0) {
    return False();
  }
  VALUE object = unwrap(info);
  VALUE methods = CALLABLE_METHODS(object);
  VALUE attr_name = V82RB((Local<Value>&)property);
  VALUE perl_name = rb_funcall(V8_To, rb_intern("perl_case"), 1, attr_name);
  
  if (RTEST(rb_ary_includes(methods, attr_name)) || RTEST(rb_ary_includes(methods, perl_name))) {
    return True();
  } else {
    return Handle<Boolean>();
  }
}

/**
 * Returns a non-empty handle if the deleter intercepts the request.
 * The return value is true if the property could be deleted and false
 * otherwise.
 */
Handle<Boolean> RacerRubyNamedPropertyDeleter(Local<String> property, const AccessorInfo& info) {
  return False();
}

/**
 * Returns an array containing the names of the properties the named
 * property getter intercepts.
 */
Handle<Array> RacerRubyNamedPropertyEnumerator(const AccessorInfo& info) {
  VALUE object = unwrap(info);
  VALUE methods = CALLABLE_METHODS(object);
  int length = RARRAY_LEN(methods);
  Local<Array> properties = Array::New(length);
  for (int i = 0; i < length; i++) {
    VALUE camel_name = rb_funcall(V8_To, rb_intern("camel_case"), 1, rb_ary_entry(methods, i));
    properties->Set(Integer::New(i), RB2V8(camel_name));
  }
  return properties;
}
