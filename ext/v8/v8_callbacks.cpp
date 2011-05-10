#include "v8_callbacks.h"
#include "rr.h"

using namespace v8;


namespace {
  VALUE ArgumentsClass;
  VALUE AccessorInfoClass;
  
  VALUE _Data(VALUE self) {
    return rb_iv_get(self, "data");
  }

  namespace Accessor {
    AccessorInfo *info(VALUE value) {
      AccessorInfo* i = 0;
      Data_Get_Struct(value, class AccessorInfo, i);
      return i;
    }
    VALUE This(VALUE self) {
      return rr_v82rb(info(self)->This());
    }
    VALUE Holder(VALUE self) {
      return rr_v82rb(info(self)->Holder());
    }
  }
  
  namespace Args {
    Arguments* args(VALUE value) {
      Arguments *arguments = 0;
      Data_Get_Struct(value, class Arguments, arguments);
      return arguments;
    }
    VALUE This(VALUE self) {
      return rr_v82rb(args(self)->This());    
    }

    VALUE Holder(VALUE self) {
      return rr_v82rb(args(self)->Holder());
    }

    VALUE Length(VALUE self) {
      return rr_v82rb(args(self)->Length());
    }
    VALUE Get(VALUE self, VALUE index) {
      int i = NUM2INT(index);
      return rr_v82rb((*args(self))[i]);
    }
    VALUE Callee(VALUE self) {
      return rr_v82rb(args(self)->Callee());
    }
    VALUE IsConstructCall(VALUE self) {
      return rr_v82rb(args(self)->IsConstructCall());
    }
  }

}

void rr_init_v8_callbacks() {
  AccessorInfoClass = rr_define_class("AccessorInfo");
  rr_define_method(AccessorInfoClass, "This", Accessor::This, 0);
  rr_define_method(AccessorInfoClass, "Holder", Accessor::Holder, 0);
  rr_define_method(AccessorInfoClass, "Data", _Data, 0);
  
  ArgumentsClass = rr_define_class("Arguments");
  rr_define_method(ArgumentsClass, "This", Args::This, 0);
  rr_define_method(ArgumentsClass, "Holder", Args::Holder, 0);
  rr_define_method(ArgumentsClass, "Data", _Data, 0);
  rr_define_method(ArgumentsClass, "Length", Args::Length, 0);
  rr_define_method(ArgumentsClass, "Callee", Args::Callee, 0);
  rr_define_method(ArgumentsClass, "IsConstructCall", Args::IsConstructCall, 0);
  rr_define_method(ArgumentsClass, "[]", Args::Get, 1);
}

VALUE rr_v82rb(const AccessorInfo& info) {
  return Data_Wrap_Struct(AccessorInfoClass, 0, 0, (void*)&info);
}
VALUE rr_v82rb(const Arguments& arguments) {
  return Data_Wrap_Struct(ArgumentsClass, 0, 0, (void*)&arguments);
}
