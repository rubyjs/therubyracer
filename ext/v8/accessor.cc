#include "rr.h"

namespace rr {

  VALUE Accessor::Class;

  void Accessor::Init() {
    ClassBuilder("AccessorInfo").
      defineMethod("This", &This).
      defineMethod("Holder", &Holder).
      defineMethod("Data", &Data).
      store(&Class);
  }

  Accessor::Accessor(const v8::AccessorInfo& info) {
    this->thisObject = Object::create(info.This());
    this->holder = Object::create(info.Holder());
    this->value = Data_Wrap_Struct(Class, &mark, &sweep, this);
    this->info = new Info(info.Data());
  }
  Accessor::~Accessor() {
    delete info;
  }
  Accessor* Accessor::accessor(VALUE wrap) {
    Accessor* info = 0;
    Data_Get_Struct(wrap, class Accessor, info);
    return info;
  }
  Accessor::operator VALUE() {
    return this->value;
  }
  VALUE Accessor::This(VALUE self) {
    return accessor(self)->thisObject;
  }

  VALUE Accessor::Holder(VALUE self) {
    return accessor(self)->holder;
  }
  VALUE Accessor::Data(VALUE self) {
    return accessor(self)->info->data;
  }

  v8::Handle<v8::Value> Accessor::get(v8::Local<v8::String> property) {
    return Value(rb_funcall(info->getter, rb_intern("call"), 2, Convert(property), this->value));
  }

  v8::Handle<v8::Value> Accessor::set(v8::Local<v8::String> property, v8::Local<v8::Value> value) {
    return Value(rb_funcall(info->setter, rb_intern("call"), 3, Convert(property), Convert(value), this->value));
  }
  void Accessor::mark(Accessor* acc) {
    rb_gc_mark(acc->thisObject);
    rb_gc_mark(acc->holder);
    acc->info->mark();
  }
  void Accessor::sweep(Accessor* acc) {
    delete acc;
  }

  v8::Handle<v8::Value> Accessor::AccessorGetter(v8::Local<v8::String> property, const v8::AccessorInfo& info) {
    Accessor* getter = new Accessor(info);
    return getter->get(property);
  }

  void Accessor::AccessorSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {
    Accessor* accessor = new Accessor(info);
    accessor->set(property, value);
  }

  Accessor::Info::Info(VALUE getter, VALUE setter, VALUE data) {
    this->getter = getter;
    this->setter = setter;
    this->data = data;
  }
  Accessor::Info::Info(v8::Local<v8::Value> data) {
    v8::Local<v8::Object> wrapper = data->ToObject();
    this->getter = unwrap(wrapper, 0);
    this->setter = unwrap(wrapper, 1);
    this->data = unwrap(wrapper, 2);
  }
  Accessor::Info::operator v8::Handle<v8::Value>() {
    v8::Local<v8::Object> wrapper = v8::Object::New();
    wrap(wrapper, 0, this->getter);
    wrap(wrapper, 1, this->setter);
    wrap(wrapper, 2, this->data);
    return wrapper;
  }
  void Accessor::Info::wrap(v8::Handle<v8::Object> wrapper, uint32_t index, VALUE value) {
    wrapper->Set(index, External::wrap(value));
  }
  VALUE Accessor::Info::unwrap(v8::Handle<v8::Object> wrapper, uint32_t index) {
    v8::Local<v8::External> holder = v8::External::Cast(*(wrapper->Get(index)));
    return External::unwrap(holder);
  }
  v8::AccessorGetter Accessor::Info::Getter() {
    return &Accessor::AccessorGetter;
  }
  v8::AccessorSetter Accessor::Info::Setter() {
    return RTEST(this->setter) ? &Accessor::AccessorSetter : 0;
  }

  void Accessor::Info::mark() {
    rb_gc_mark(getter);
    rb_gc_mark(setter);
    rb_gc_mark(data);
  }
}