#ifndef RR_ARRAY
#define RR_ARRAY

namespace rr {

  class Array : public Ref<v8::Array> {
  public:
    static void Init();

    static VALUE New(int argc, VALUE argv[], VALUE self);
    static VALUE Length(VALUE self);
    static VALUE CloneElementAt(VALUE self, VALUE index);

    inline Array(v8::Isolate* isolate, v8::Handle<v8::Array> array) : Ref<v8::Array>(isolate, array) {}
    inline Array(VALUE value) : Ref<v8::Array>(value) {}

    typedef MaybeLocal<Array, v8::Array> Maybe;
  };

}

#endif
