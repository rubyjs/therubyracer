#include "rr.h"
#include "v8_external.h"

#include "v8_handle.h"
#include "v8_value.h"
using namespace v8;

namespace {
  VALUE ExternalClass;
  VALUE references;

  struct Weaklink {
    bool finalized_from_rb;
    bool finalized_from_v8;
    VALUE target;
  };

  VALUE Weaklink_finalized_from_ruby(VALUE object_id, VALUE data) {
    Weaklink* link = 0;
    Data_Get_Struct(data, struct Weaklink, link);
    link->finalized_from_rb = true;
    if (link->finalized_from_v8) {
      delete link;
    }
    return Qnil;
  }
  void Weaklink_finalized_from_v8(Persistent<Value> value, void* data) {
    Weaklink* link = (Weaklink*)data;
    link->finalized_from_v8 = true;
    if (link->finalized_from_rb) {
      delete link;
    }
    value.Dispose();
  }

  VALUE New(VALUE self, VALUE value) {
    HandleScope scope;
    Weaklink* link = new Weaklink();
    link->finalized_from_v8 = false;
    link->finalized_from_rb = false;
    link->target = value;
    Persistent<External> external = Persistent<External>::New(External::New((void*)link));
    external.MakeWeak(link,Weaklink_finalized_from_v8);
    VALUE finalizer_data = Data_Wrap_Struct(rb_cObject, 0, 0, link);
    rr_define_finalizer(value, (void*)Weaklink_finalized_from_ruby, finalizer_data);
    return rr_v8_handle_new(self, external);
  }

  VALUE _Value(VALUE self) {
    HandleScope scope;
    Weaklink* link = (Weaklink*)rr_v8_handle<External>(self)->Value();
    if (link->finalized_from_rb) {
      rb_warn("an active v8::External wraps a VALUE that has already been finalized! This is not good.\n");
      return Qnil;
    } else {
      return link->target;
    }
    return (VALUE)rr_v8_handle<External>(self)->Value();
  }
}

void rr_init_v8_external() {
  ExternalClass = rr_define_class("External", rr_v8_value_class());
  rr_define_singleton_method(ExternalClass, "New", New, 1);
  rr_define_method(ExternalClass, "Value", _Value, 0);
}

VALUE rr_reflect_v8_external(Handle<Value> external) {
  return rr_v8_handle_new(ExternalClass, external);
}
