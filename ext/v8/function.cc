#include "rr.h"
#include "pthread.h"
#include "unistd.h"

namespace rr {
  void Function::Init() {
    ClassBuilder("Function", Object::Class).
      defineMethod("NewInstance", &NewInstance).
      defineMethod("Call", &Call).
      defineMethod("CallWithTimeout", &CallWithTimeout).
      defineMethod("SetName", &SetName).
      defineMethod("GetName", &GetName).
      defineMethod("GetInferredName", &GetInferredName).
      defineMethod("GetScriptLineNumber", &GetScriptLineNumber).
      defineMethod("GetScriptColumnNumber", &GetScriptColumnNumber).
      defineMethod("GetScriptId", &GetScriptId).
      defineMethod("GetScriptOrigin", &GetScriptOrigin).
      store(&Class);
  }

  VALUE Function::NewInstance(int argc, VALUE argv[], VALUE self) {
    VALUE args;
    rb_scan_args(argc,argv,"01", &args);
    if (RTEST(args)) {
      return Object(Function(self)->NewInstance(RARRAY_LENINT(args), Value::array<Value>(args)));
    } else {
      return Object(Function(self)->NewInstance());
    }
  }
  VALUE Function::Call(VALUE self, VALUE receiver, VALUE argv) {
    return Value(Function(self)->Call(Object(receiver), RARRAY_LENINT(argv), Value::array<Value>(argv)));
  }

  typedef struct {
      v8::Isolate *isolate;
      long timeout;
  } fct_timeout_data;

  void* fct_breaker(void *d) {
    fct_timeout_data* data = (fct_timeout_data*)d;
    usleep(data->timeout*1000);
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    v8::V8::TerminateExecution(data->isolate);
    return NULL;
  }

  VALUE Function::CallWithTimeout(VALUE self, VALUE receiver, VALUE argv, VALUE timeout) {
    pthread_t fct_breaker_thread;
    fct_timeout_data data;
    VALUE rval;
    void *res;

    data.isolate = v8::Isolate::GetCurrent();
    data.timeout = NUM2LONG(timeout);

    pthread_create(&fct_breaker_thread, NULL, fct_breaker, &data);

    rval = Value(Function(self)->Call(Object(receiver), RARRAY_LENINT(argv), Value::array<Value>(argv)));

    pthread_cancel(fct_breaker_thread);
    pthread_join(fct_breaker_thread, &res);

    return rval;
  }

  VALUE Function::SetName(VALUE self, VALUE name) {
    Void(Function(self)->SetName(String(name)));
  }

  VALUE Function::GetName(VALUE self) {
    return Value(Function(self)->GetName());
  }

  VALUE Function::GetInferredName(VALUE self) {
    return Value(Function(self)->GetInferredName());
  }

  VALUE Function::GetScriptLineNumber(VALUE self) {
    return INT2FIX(Function(self)->GetScriptLineNumber());
  }

  VALUE Function::GetScriptColumnNumber(VALUE self) {
    return INT2FIX(Function(self)->GetScriptColumnNumber());
  }

  VALUE Function::GetScriptId(VALUE self) {
    return Value(Function(self)->GetScriptId());
  }

  VALUE Function::GetScriptOrigin(VALUE self) {
    return not_implemented("GetScriptOrigin");
  }
}