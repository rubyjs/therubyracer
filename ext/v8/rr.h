#ifndef THE_RUBY_RACER
#define THE_RUBY_RACER

#include <include/v8.h>
#include <include/libplatform/libplatform.h>

#include <ruby.h>
#include <vector>
#ifdef HAVE_RUBY_ENCODING_H
#include "ruby/encoding.h"
#endif

#include "class_builder.h"
#include "pointer.h"
#include "ref.h"

#include "v8.h"
#include "isolate.h"
#include "handles.h"
#include "context.h"

#include "equiv.h"
#include "bool.h"
#include "uint32.h"
#include "value.h"
#include "backref.h"

#include "object.h"

#endif
