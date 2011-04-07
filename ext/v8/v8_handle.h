#ifndef _RR_V8_HANDLE_
#define _RR_V8_HANDLE_ 

void rr_init_handle();
VALUE rr_v8_handle_new(v8::Local<void>& handle);
VALUE rr_v8_handle_new(v8::Persistent<void>& handle);

#endif
