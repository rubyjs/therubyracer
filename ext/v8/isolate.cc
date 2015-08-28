// -*- mode: c++ -*-
#include "rr.h"

namespace rr {

  VALUE Isolate::Class;

  void Isolate::Init() {
    rb_eval_string("require 'v8/retained_objects'");
    ClassBuilder("Isolate").
      defineSingletonMethod("New", &New).
      defineMethod("ThrowException", &ThrowException).
      defineMethod("SetCaptureStackTraceForUncaughtExceptions", &SetCaptureStackTraceForUncaughtExceptions).
      defineMethod("IdleNotificationDeadline", &IdleNotificationDeadline).
      defineMethod("RequestGarbageCollectionForTesting", &RequestGarbageCollectionForTesting).
      defineMethod("__EachV8Finalizer__", &__EachV8Finalizer__).
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
    isolate->AddGCPrologueCallback(&clearReferences, v8::kGCTypeAll);

    data->isolate = isolate;
    return Isolate(isolate);
  }

  VALUE Isolate::ThrowException(VALUE self, VALUE error) {
    Isolate isolate(self);
    Locker lock(isolate);
    return Value(isolate, isolate->ThrowException(Value(error)));
  }

  VALUE Isolate::SetCaptureStackTraceForUncaughtExceptions(VALUE self, VALUE capture, VALUE stack_limit, VALUE options) {
    Isolate isolate(self);
    Locker lock(isolate);

    isolate->SetCaptureStackTraceForUncaughtExceptions(
      Bool(capture),
      RTEST(stack_limit) ? NUM2INT(stack_limit) : 10,
      Enum<v8::StackTrace::StackTraceOptions>(options, v8::StackTrace::kOverview));
    return Qnil;
  }

  VALUE Isolate::IdleNotificationDeadline(VALUE self, VALUE deadline_in_seconds) {
    Isolate isolate(self);
    Locker lock(isolate);
    return Bool(isolate->IdleNotificationDeadline(NUM2DBL(deadline_in_seconds)));
  }

  VALUE Isolate::RequestGarbageCollectionForTesting(VALUE self) {
    Isolate isolate(self);
    Locker lock(isolate);
    isolate->RequestGarbageCollectionForTesting(v8::Isolate::kFullGarbageCollection);
    return Qnil;
  }
  VALUE Isolate::__EachV8Finalizer__(VALUE self) {
    if (!rb_block_given_p()) {
      rb_raise(rb_eArgError, "Expected block");
      return Qnil;
    }
    int state(0);
    {
      Isolate isolate(self);
      isolate.eachV8Finalizer(&state);
    }
    if (state != 0) {
      rb_jump_tag(state);
    }
    return Qnil;
  }
}
