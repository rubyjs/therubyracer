#include <ruby.h>

#include "callbacks.h"
#include "converters.h"

using namespace v8;

Handle<Value> RacerRubyInvocationCallback(const Arguments& args) {
  VALUE code = (VALUE)External::Unwrap(args.Data());
  if (NIL_P(code)) {
    return Null();
  } else {      
    VALUE* arguments = new VALUE[args.Length()];
    for(int c=0;c<args.Length(); ++c) {
      Handle<Value> val = args[c];
      arguments[c] = V82RB(val);
    }
      
    VALUE result = rb_funcall2(code, rb_intern("call"), args.Length(), arguments);
    delete [] arguments;
    
    Handle<Value> convertedResult = RB2V8(result);
    return convertedResult  ;
  }
}