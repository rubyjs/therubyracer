#ifndef RR_STRING
#define RR_STRING

namespace rr {

  class String: public Ref<v8::String> {
  public:
    static void Init();

    static VALUE NewFromUtf8(VALUE self, VALUE isolate, VALUE value);
    static VALUE Utf8Value(VALUE self);
    static VALUE Concat(VALUE self, VALUE left, VALUE right);

    inline String(VALUE value) : Ref<v8::String>(value) {}
    inline String(v8::Isolate* isolate, v8::Handle<v8::String> string) : Ref<v8::String>(isolate, string) {}

    virtual operator v8::Handle<v8::String>() const;
  };

}

#endif
