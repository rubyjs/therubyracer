// -*- mode: c++ -*-
#include "rr.h"
#include "isolate.h"

namespace rr {

  VALUE Isolate::Class;

  void Isolate::Init() {
    rb_eval_string("require 'v8/retained_objects'");
    ClassBuilder("Isolate").
      defineSingletonMethod("New", &New).

      defineMethod("IdleNotificationDeadline", &IdleNotificationDeadline).

      store(&Class);
  }

  VALUE Isolate::New(VALUE self) {
    Isolate::IsolateData* data = new IsolateData();

    VALUE rb_cRetainedObjects = rb_eval_string("V8::RetainedObjects");
    data->retained_objects = rb_funcall(rb_cRetainedObjects, rb_intern("new"), 0);

    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = &data->array_buffer_allocator;
    v8::Isolate* isolate = v8::Isolate::New(create_params);

    isolate->SetData(0, data);
    isolate->AddGCPrologueCallback(&clearReferences);

    data->isolate = isolate;
    return Isolate(isolate);
  }


  VALUE Isolate::IdleNotificationDeadline(VALUE self, VALUE deadline_in_seconds) {
    Isolate isolate(self);
    Locker lock(isolate);
    return Bool(isolate->IdleNotificationDeadline(NUM2DBL(deadline_in_seconds)));
  }
}
