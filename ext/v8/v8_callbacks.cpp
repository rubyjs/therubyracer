#include "v8_callbacks.h"
#include "rr.h"

using namespace v8;


namespace {
  VALUE ArgumentsClass;
  VALUE AccessorInfoClass;
    
  struct Wrap {
    Persistent<Object> thisObj;
    Persistent<Object> holder;
    Persistent<Value> data;
    Wrap(Handle<Object> thisObj, Handle<Object> holder, Handle<Value> data);
    virtual ~Wrap();
  };
  
  Wrap::Wrap(Handle<Object> thisObj_, Handle<Object> holder_, Handle<Value> data_) : 
    thisObj(Persistent<Object>::New(thisObj_)), 
    holder(Persistent<Object>::New(holder_)), 
    data(Persistent<Value>::New(data_)) {}
  Wrap::~Wrap() {
    thisObj.Dispose();
    holder.Dispose();
    data.Dispose();
  }
  void gc_wrap_mark(Wrap *wrapper) {}
  void gc_wrap_free(Wrap *wrapper) {
    delete wrapper;
  }
  
  struct WrapArguments : Wrap {
    int length;
    Persistent<Function> callee;
    bool isConstructCall;
    Persistent<Array> values;
    WrapArguments(const Arguments& arguments);
    virtual ~WrapArguments();    
  };
  
  WrapArguments::WrapArguments(const Arguments& arguments) : Wrap(arguments.This(), arguments.Holder(), arguments.Data()),
    callee(Persistent<Function>::New(arguments.Callee())),
    values(Persistent<Array>::New(Array::New(arguments.Length()))) {
    length = arguments.Length();
    isConstructCall = arguments.IsConstructCall();
    for (int i = 0; i < arguments.Length(); i++) {
      values->Set(Integer::New(i), arguments[i]);
    }
  }
    
  WrapArguments::~WrapArguments() {
    callee.Dispose();
    values.Dispose();
  }
  
  Wrap *info(VALUE value) {
    Wrap *wrap = 0;
    Data_Get_Struct(value, struct Wrap, wrap);
    return wrap;
  }
  
  WrapArguments* args(VALUE value) {
    WrapArguments *wrap = 0;
    Data_Get_Struct(value, struct WrapArguments, wrap);
    return wrap;
  }
  
  VALUE Length(VALUE self) {
    return rr_v82rb(args(self)->length);
  }
  VALUE Get(VALUE self, VALUE index) {
    int i = NUM2INT(index);
    return rr_v82rb(args(self)->values->Get(i));
  }
  VALUE Callee(VALUE self) {
    return rr_v82rb(args(self)->callee);
  }
  VALUE This(VALUE self) {
    return rr_v82rb(info(self)->thisObj);    
  }
  VALUE Holder(VALUE self) {
    return rr_v82rb(info(self)->holder);    
  }
  VALUE IsConstructCall(VALUE self) {
    return rr_v82rb(args(self)->isConstructCall);
  }
  VALUE _Data(VALUE self) {
    return rr_v82rb(info(self)->data);
  }
  
  struct WrapAccessorInfo : Wrap {
    WrapAccessorInfo(const AccessorInfo& info);
  };
  WrapAccessorInfo::WrapAccessorInfo(const AccessorInfo& info) : Wrap(info.This(), info.Holder(), info.Data()) {}
  
}

void rr_init_v8_callbacks() {
  AccessorInfoClass = rr_define_class("AccessorInfo");
  rr_define_method(AccessorInfoClass, "This", This, 0);
  rr_define_method(AccessorInfoClass, "Holder", Holder, 0);
  rr_define_method(AccessorInfoClass, "Data", _Data, 0);
  
  ArgumentsClass = rr_define_class("Arguments");
  rr_define_method(ArgumentsClass, "This", This, 0);
  rr_define_method(ArgumentsClass, "Holder", Holder, 0);
  rr_define_method(ArgumentsClass, "Data", _Data, 0);
  rr_define_method(ArgumentsClass, "Length", Length, 0);
  rr_define_method(ArgumentsClass, "Callee", Callee, 0);
  rr_define_method(ArgumentsClass, "IsConstructCall", IsConstructCall, 0);
  rr_define_method(ArgumentsClass, "[]", Get, 1);
}

VALUE rr_v82rb(const AccessorInfo& info) {
  return Data_Wrap_Struct(AccessorInfoClass, gc_wrap_mark, gc_wrap_free, new WrapAccessorInfo(info));
}

VALUE rr_v82rb(const Arguments& arguments) {
  return Data_Wrap_Struct(ArgumentsClass, gc_wrap_mark, gc_wrap_free, new WrapArguments(arguments));  
}
