#include "rr.h"

namespace rr {

  VALUE Accessor::Info::Class;

  void Accessor::Init() {
    ClassBuilder("AccessorInfo").
      defineMethod("This", &Info::This).
      defineMethod("Holder", &Info::Holder).
      defineMethod("Data", &Info::Data).
      store(&Info::Class);
  }

  Accessor::Accessor(VALUE get, VALUE set, VALUE data) {
    this->get = get;
    this->set = set;
    this->data = data;
  }

  Accessor::Accessor(v8::Handle<v8::Value> value) {
    v8::Local<v8::Object> wrapper = value->ToObject();
    this->get = unwrap(wrapper, 0);
    this->set = unwrap(wrapper, 1);
    v8::Handle<v8::Value> data = wrapper->Get(2);
    if (!data.IsEmpty()) {
      this->data = Value(data);
    }
  }

  Accessor::operator v8::AccessorGetter() {
    return &Getter;
  }

  Accessor::operator v8::AccessorSetter() {
    return RTEST(this->set) ? &Setter : 0;
  }

  Accessor::operator v8::Handle<v8::Value>() {
    v8::Local<v8::Object> wrapper = v8::Object::New();
    wrap(wrapper, 0, this->get);
    wrap(wrapper, 1, this->set);
    if (RTEST(this->data)) {
      wrapper->Set(2, Value(this->data));
    }
    return wrapper;
  }

  void Accessor::wrap(v8::Handle<v8::Object> wrapper, int index, VALUE value) {
    wrapper->Set(index, External::wrap(value));
  }

  VALUE Accessor::unwrap(v8::Handle<v8::Object> wrapper, int index) {
    v8::Handle<v8::External> external(v8::External::Cast(*wrapper->Get(index)));
    return External::unwrap(external);
  }

  VALUE Accessor::Info::This(VALUE self) {
    return Object(Info(self)->This());
  }

  VALUE Accessor::Info::Holder(VALUE self) {
    return Object(Info(self)->Holder());
  }

  VALUE Accessor::Info::Data(VALUE self) {
    return Accessor(Info(self)->Data()).data;
  }

  v8::Handle<v8::Value> Accessor::Getter(v8::Local<v8::String> property, const v8::AccessorInfo& info) {
    return Info(info).get(property);
  }

  void Accessor::Setter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {
    Info(info).set(property, value);
  }

  Accessor::Info::Info(const v8::AccessorInfo& info) {
    this->info = &info;
  }

  Accessor::Info::Info(VALUE value) {
    Data_Get_Struct(value, class v8::AccessorInfo, info);
  }

  v8::Handle<v8::Value> Accessor::Info::get(v8::Local<v8::String> property) {
    Accessor accessor(info->Data());
    return Value(rb_funcall(accessor.get, rb_intern("call"), 2, (VALUE)String(property), (VALUE)*this));
  }

  void Accessor::Info::set(v8::Local<v8::String> property, v8::Local<v8::Value> value) {
    Accessor accessor(info->Data());
    rb_funcall(accessor.set, rb_intern("call"), 3, (VALUE)String(property), (VALUE)Value(value), (VALUE)*this);
  }
  Accessor::Info::operator VALUE() {
    return Data_Wrap_Struct(Class, 0, 0, (void*)this->info);
  }
}