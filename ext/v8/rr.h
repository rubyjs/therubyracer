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
#include "maybe.h"
#include "equiv.h"
#include "bool.h"
#include "pointer.h"
#include "isolate.h"

#include "ref.h"

#include "v8.h"
#include "locks.h"
#include "handles.h"
#include "context.h"

#include "uint32.h"
#include "value.h"
#include "backref.h"

#include "object.h"
#include "array.h"
#include "primitive.h"
#include "external.h"
// This one is named v8_string to avoid name collisions with C's string.h
#include "name.h"
#include "rr_string.h"
#include "symbol.h"

#include "script.h"
#include "script-origin.h"
#include "function.h"

#endif
