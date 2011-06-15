#include "v8_handle.h"
#include "v8_context.h"
#include "v8_value.h"
#include "v8_string.h"
#include "v8_object.h"
#include "v8_array.h"
#include "v8_message.h"
#include "v8_function.h"
#include "v8_date.h"
#include "v8_script.h"
#include "v8_template.h"
#include "v8_try_catch.h"
#include "v8_callbacks.h"
#include "v8_external.h"
#include "v8_exception.h"
#include "v8_locker.h"
#include "v8_debug.h"
#include "v8_v8.h"

#include <stdio.h>

extern "C" {
  void Init_v8();
}

extern "C" {
  void Init_v8() {
    v8::Locker locker;
    rr_init_handle();
    rr_init_context();
    rr_init_value();
    rr_init_string();
    rr_init_script();
    rr_init_template();
    rr_init_object();
    rr_init_function();
    rr_init_v8_array();
    rr_init_v8_date();
    rr_init_message();
    rr_init_v8_try_catch();
    rr_init_v8_callbacks();
    rr_init_v8_external();
    rr_init_v8_exception();
    rr_init_v8_locker();
    rr_init_v8_debug();
    rr_init_v8_v8();
  }
}
