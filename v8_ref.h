#ifndef _RUBY_V8_REF_
#define _RUBY_V8_REF_

#include <v8.h>

class v8_ref_data {
  
};

template <class T> class v8_ref : v8_ref_data {
public:
  v8_ref(v8::Handle<T> object);
  ~v8_ref();
private:
  v8::Persistent<T> handle;
};


//memory management
void v8_ref_mark(v8_ref_data* ref);
void v8_ref_free(v8_ref_data* ref);


#endif