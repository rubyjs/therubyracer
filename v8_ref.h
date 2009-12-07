#ifndef _RUBY_V8_REF_
#define _RUBY_V8_REF_

#include <v8.h>

//this is a non-template superclass so that the mark() and free() functions
//don't need to be templatized. Not sure if this is valid... are
// class destructors virtual?

class v8_ref_data {
  
};

//the v8_ref wraps a v8 handle so that ruby can hold a reference to it.

template <class T> class v8_ref : v8_ref_data {
public:
  //takes a handle object and adds a new persistent handle for 
  //the referenced object.
  v8_ref(v8::Handle<T> object);
  ~v8_ref();
private:
  v8::Persistent<T> handle;
};


//memory management
void v8_ref_mark(v8_ref_data* ref);
void v8_ref_free(v8_ref_data* ref);


#endif