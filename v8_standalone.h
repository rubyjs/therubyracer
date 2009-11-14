#ifndef __v8_standalone_h__
#define __v8_standalone_h__

#include "ruby.h"

/**
 * interned symbol for "call"
 */
extern VALUE ruby_call_symbol;

/**
 * id for respond_to?
 */
extern VALUE ruby_respond_to_ID;

/**
 * Determine whether or not a value can respond to "call".
 * i.e. is this Proc/Method object?
 */
bool is_callable(VALUE& object);

/**
 * Debugging aid.  Println debugging goo for VALUES.
 */
VALUE v8_what_is_this(VALUE self, VALUE object);

#endif