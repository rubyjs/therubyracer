// -*- mode: c++ -*-
#include "rr.h"
#include "isolate.h"

namespace rr {

  void Isolate::Init() {
    rb_eval_string("require 'v8/retained_objects'");
    ClassBuilder("Isolate").
      defineSingletonMethod("New", &New).

      defineMethod("Dispose", &Isolate::Dispose).
      defineMethod("Equals", &rr::Isolate::PointerEquals).

      store(&Class);
  }


  VALUE Isolate::New(VALUE self) {
    Isolate::IsolateData* data = new IsolateData();
    VALUE rb_cRetainedObjects = rb_eval_string("V8::RetainedObjects");
    data->retained_objects = rb_funcall(rb_cRetainedObjects, rb_intern("new"), 0);

    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = &data->array_buffer_allocator;

    Isolate isolate(v8::Isolate::New(create_params));
    isolate->SetData(0, data);
    isolate->AddGCPrologueCallback(&clearReferences);

    return isolate;
  }

  VALUE Isolate::Dispose(VALUE self) {
    Isolate isolate(self);
    delete isolate.data();
    isolate->Dispose();
    return Qnil;
  }

  template <>
  void Pointer<v8::Isolate>::unwrap(VALUE value) {
    Data_Get_Struct(value, class v8::Isolate, pointer);
  }
}
