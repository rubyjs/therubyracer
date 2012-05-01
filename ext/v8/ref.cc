#include "rr.h"

namespace rr {
  template<typename T>
  Ref<T> Ref<T>::create(v8::Handle<T> handle, VALUE klass) {
    return Ref<T>(new Holder(handle, klass));
  }
  template<typename T>
  Ref<T>::Ref(Ref<T>::Holder* holder) {
    this->holder = holder;
  }
  template<typename T>
  Ref<T>::Ref(VALUE wrapper) {
    Holder* holder = NULL;
    Data_Get_Struct(wrapper, class Holder, holder) ;
    this->holder = holder;
  }
  template<typename T>
  Ref<T>::operator VALUE() {
    return holder->value;
  }
  template<typename T>
  Ref<T>::Holder::Holder(v8::Handle<T> handle, VALUE klass) {
    this->handle = v8::Persistent<T>::New(handle);
    this->value = Data_Wrap_Struct(klass, 0, &Holder::enqueue, this);
  }
  template<typename T>
  Ref<T>::Holder::~Holder() {
    this->handle.Dispose();
  }
  template<typename T>
  void Ref<T>::Holder::enqueue(Holder* holder) {
    holder->value = Qnil;
    printf("enqueuing a reference for garbage collection (should actually do something)\n");
  }
}

