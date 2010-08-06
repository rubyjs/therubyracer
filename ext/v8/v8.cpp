#include "v8_cxt.h"
#include "v8_value.h"
#include "v8_str.h"
#include "v8_obj.h"
#include "v8_array.h"
#include "v8_msg.h"
#include "v8_func.h"
#include "v8_date.h"
#include "v8_script.h"
#include "v8_template.h"
#include "v8_try_catch.h"
#include "v8_callbacks.h"
#include "v8_external.h"
#include "v8_exception.h"
#include "v8_locker.h"

#include <stdio.h>

extern "C" {
    void Init_v8();
}

extern "C" {
    void Init_v8() {
        rr_init_cxt();
        rr_init_value();
        rr_init_str();
        rr_init_script();
        rr_init_template();
        rr_init_obj();
        rr_init_func();
        rr_init_v8_array();
        rr_init_v8_date();
        rr_init_msg();
        rr_init_v8_try_catch();
        rr_init_v8_callbacks();
        rr_init_v8_external();
        rr_init_v8_exception();
        rr_init_v8_locker();
    }
}
