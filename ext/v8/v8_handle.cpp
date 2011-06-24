
#include "rr.h"
#include "v8_handle.h"

using namespace v8;

/**
* Creates a new Persistent storage cell for `handle`
* so that we can reference it from Ruby. Ruby metadat
* is contained on the handle object, and then the actual
* v8 references are contain in an instance of `Payload`
*/
v8_handle::v8_handle(Handle<void> object) {
  this->weakref_callback = Qnil;
  this->weakref_callback_parameters = Qnil;
  this->dead = false;
  this->payload = new Payload(object);
}

v8_handle::~v8_handle() {}

/**
* Construct a new handle payload.
*
* Each payload contains a Ruby object wrapper so that it
* can be enqueued for V8 GC (the GC queue is a ruby Array)
* the wrapper is pre-allocated at payload creation time
* so that no Ruby objects are allocated during Ruby GC.
*/
v8_handle::Payload::Payload(Handle<void> object) {
  rb_gc_register_address(&wrapper);
  handle = Persistent<void>::New(object);
  wrapper = Data_Wrap_Struct(rb_cObject, 0, destroy, this);
}

v8_handle::Payload::~Payload() {
  rb_gc_unregister_address(&wrapper);
}

void v8_handle::Payload::release() {
  handle.Dispose();
  handle.Clear();
}

void v8_handle::Payload::destroy(v8_handle::Payload* payload) {
  delete payload;
}

namespace {
  /**
  * Holds dead references, that are no longer being held in Ruby, so that they can be garbage collected
  * inside of V8
  */
  VALUE handle_queue;

  /**
  * Invoked by the Ruby garbage collector whenever it determines that this handle is
  * still reachable. We in turn, mark our weak callback parameters, so that it knows
  * they are reachable too.
  */
  void v8_handle_mark(v8_handle* handle) {
    rb_gc_mark(handle->weakref_callback);
    rb_gc_mark(handle->weakref_callback_parameters);
  }

  /**
  * Whenver a V8::C::Handle becomes garbage collected, we do not free it immediately.
  * instead, we put them into a "zombie" queue, where its corresponding V8 storage cell
  * can be released safely while the V8 engine is running. A zombie Ruby object is
  * created to wrap it so that it can be stored in the queue.
  */
  void v8_handle_enqueue(v8_handle* handle) {
    handle->dead = true;
    rb_ary_unshift(handle_queue, handle->payload->wrapper);
  }

  /**
  * Drains the dead handle queue, and releases them from V8
  *
  * This implements the V8 `GCPrologueCallback` and is registered to run before
  * each invocation of the V8 garbage collector. It empties the queue of dead handles
  * and disposes of them. It is important to do this operations inside V8 so that
  * Ruby garbage collection is never locked, and never touches V8.
  */
  void v8_handle_dequeue(GCType type, GCCallbackFlags flags) {
    for (VALUE handle = rb_ary_pop(handle_queue); RTEST(handle); handle = rb_ary_pop(handle_queue)) {
      v8_handle::Payload* payload = NULL;
      Data_Get_Struct(handle, struct v8_handle::Payload, payload);
      payload->release();
    }
  }

  VALUE New(VALUE self, VALUE handle) {
    if (RTEST(handle)) {
      Persistent<void> that = rr_v8_handle<void>(handle);
      return rr_v8_handle_new(self, that);
    } else {
      return rr_v8_handle_new(self, Handle<void>());
    }
  }

  VALUE IsEmpty(VALUE self) {
    return rr_v82rb(rr_v8_handle<void>(self).IsEmpty());
  }

  VALUE Clear(VALUE self) {
    rr_v8_handle<void>(self).Clear();
    return Qnil;
  }

  VALUE Dispose(VALUE self) {
    rr_v8_handle<void>(self).Dispose();
    return Qnil;
  }

  void RubyWeakReferenceCallback(Persistent<Value> value, void* parameter) {
    VALUE self = (VALUE)parameter;
    v8_handle* handle = rr_v8_handle_raw(self);
    VALUE callback = handle->weakref_callback;
    VALUE parameters = handle->weakref_callback_parameters;
    if (RTEST(callback)) {
      rb_funcall(callback, rb_intern("call"), 2, self, parameters);
    }
    value.Dispose();
    handle->payload->release();
    handle->dead = true;

  }

  VALUE MakeWeak(VALUE self, VALUE parameters, VALUE callback) {
    v8_handle* handle = rr_v8_handle_raw(self);
    handle->weakref_callback = callback;
    handle->weakref_callback_parameters = parameters;
    rr_v8_handle<void>(self).MakeWeak((void*)self, RubyWeakReferenceCallback);
    return Qnil;
  }

  VALUE ClearWeak(VALUE self) {
    rr_v8_handle<void>(self).ClearWeak();
    return Qnil;
  }

  VALUE IsNearDeath(VALUE self) {
    return rr_v82rb(rr_v8_handle<void>(self).IsNearDeath());
  }

  VALUE IsWeak(VALUE self) {
    return rr_v82rb(rr_v8_handle<void>(self).IsWeak());
  }

  VALUE dead_p(VALUE self) {
    return rr_v8_handle_raw(self)->dead ? Qtrue : Qfalse;
  }
}

void rr_init_handle() {
  VALUE HandleClass = rr_define_class("Handle");
  rr_define_method(HandleClass, "dead?", dead_p, 0);
  rr_define_singleton_method(HandleClass, "New", New, 1);
  rr_define_method(HandleClass, "IsEmpty", IsEmpty, 0);
  rr_define_method(HandleClass, "Clear", Clear, 0);
  rr_define_method(HandleClass, "Dispose", Dispose, 0);
  rr_define_method(HandleClass, "MakeWeak", MakeWeak, 2);
  rr_define_method(HandleClass, "ClearWeak", ClearWeak, 0);
  rr_define_method(HandleClass, "IsNearDeath", IsNearDeath, 0);
  rr_define_method(HandleClass, "IsWeak", IsWeak, 0);

  rb_gc_register_address(&handle_queue);
  handle_queue = rb_ary_new();
  V8::AddGCPrologueCallback(v8_handle_dequeue);
}

VALUE rr_v8_handle_new(VALUE klass, v8::Handle<void> handle) {
  v8_handle* new_handle = new v8_handle(handle);
  return Data_Wrap_Struct(klass, v8_handle_mark, v8_handle_enqueue, new_handle);
}

VALUE rr_v8_handle_class() {
  return rr_define_class("Handle");
}

v8_handle* rr_v8_handle_raw(VALUE value) {
  v8_handle* handle = 0;
  Data_Get_Struct(value, struct v8_handle, handle);
  return handle;
}
