#ifndef THE_RUBY_RACER
#define THE_RUBY_RACER

#include <include/v8.h>
#include <include/libplatform/libplatform.h>

#include <ruby.h>
#include <vector>
#ifdef HAVE_RUBY_ENCODING_H
#include "ruby/encoding.h"
#endif

inline VALUE not_implemented(const char* message) {
  rb_raise(rb_eStandardError, "not yet implemented %s", message);

  return Qnil;
}

#include "class_builder.h"
#include "enum.h"

#include "maybe.h"
#include "equiv.h"
#include "bool.h"
#include "uint32_t.h"
#include "pointer.h"
#include "wrapper.h"
#include "isolate.h"

#include "ref.h"

#include "v8.h"
#include "locks.h"
#include "handles.h"
#include "context.h"

#include "value.h"
#include "boolean.h"

#include "primitive.h"
#include "undefined.h"
#include "null.h"
#include "number.h"
#include "integer.h"

#include "external.h"
#include "name.h"
// This one is named v8_string to avoid name collisions with C's string.h
#include "rr_string.h"
#include "symbol.h"

#include "function.h"

#include "object.h"
#include "return-value.h"
#include "property-callback.h"
#include "array.h"

#include "script.h"
#include "script-origin.h"
#include "function-callback.h"

#endif
