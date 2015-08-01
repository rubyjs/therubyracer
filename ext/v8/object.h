#ifndef RR_OBJECT
#define RR_OBJECT

namespace rr {

  class Object : public Ref<v8::Object> {
  public:
    static void Init();

    static VALUE New(VALUE self, VALUE rb_isolate);
    static VALUE Set(VALUE self, VALUE r_context, VALUE key, VALUE value);
    static VALUE Get(VALUE self, VALUE r_context, VALUE key);
    static VALUE GetIdentityHash(VALUE self);
    static VALUE Has(VALUE self, VALUE r_context, VALUE key);
    static VALUE Delete(VALUE self, VALUE r_context, VALUE key);
    static VALUE SetAccessor(int argc, VALUE* argv, VALUE self);

    static VALUE CreateDataProperty(VALUE self, VALUE r_context, VALUE key, VALUE value);

    inline Object(VALUE value) : Ref<v8::Object>(value) {}
    inline Object(v8::Isolate* isolate, v8::Handle<v8::Object> object) : Ref<v8::Object>(isolate, object) {}

    typedef MaybeLocal<Object, v8::Object> Maybe;

    operator VALUE();
  };

}

#endif
