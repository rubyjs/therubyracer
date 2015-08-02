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
    static VALUE SetAccessorProperty(int argc, VALUE* argv, VALUE self);

    static VALUE CreateDataProperty(VALUE self, VALUE r_context, VALUE key, VALUE value);
    static VALUE DefineOwnProperty(int argc, VALUE* argv, VALUE self);
    static VALUE GetPropertyAttributes(VALUE self, VALUE r_context, VALUE key);
    static VALUE GetOwnPropertyDescriptor(VALUE self, VALUE r_context, VALUE key);
    static VALUE GetPropertyNames(VALUE self, VALUE r_context);
    static VALUE GetOwnPropertyNames(VALUE self, VALUE r_context);
    static VALUE GetPrototype(VALUE self);
    static VALUE SetPrototype(VALUE self, VALUE r_context, VALUE prototype);

    static VALUE ObjectProtoToString(VALUE self, VALUE r_context);
    static VALUE GetConstructorName(VALUE self);

    static VALUE InternalFieldCount(VALUE self);
    static VALUE GetInternalField(VALUE self, VALUE index);
    static VALUE SetInternalField(VALUE self, VALUE index, VALUE value);

    static VALUE HasOwnProperty(VALUE self, VALUE r_context, VALUE key);
    static VALUE HasRealNamedProperty(VALUE self, VALUE r_context, VALUE key);
    static VALUE HasRealIndexedProperty(VALUE self, VALUE r_context, VALUE index);
    static VALUE HasRealNamedCallbackProperty(VALUE self, VALUE r_context, VALUE key);
    static VALUE GetRealNamedPropertyInPrototypeChain(VALUE self, VALUE r_context, VALUE key);
    static VALUE GetRealNamedProperty(VALUE self, VALUE r_context, VALUE key);
    static VALUE GetRealNamedPropertyAttributes(VALUE self, VALUE r_context, VALUE key);
    static VALUE GetRealNamedPropertyAttributesInPrototypeChain(VALUE self, VALUE r_context, VALUE key);

    static VALUE HasNamedLookupInterceptor(VALUE self);
    static VALUE HasIndexedLookupInterceptor(VALUE self);

    static VALUE SetHiddenValue(VALUE self, VALUE key, VALUE value);
    static VALUE GetHiddenValue(VALUE self, VALUE key);
    static VALUE DeleteHiddenValue(VALUE self, VALUE key);

    static VALUE Clone(VALUE self);
    static VALUE CreationContext(VALUE self);

    static VALUE IsCallable(VALUE self);

    static VALUE CallAsFunction(int argc, VALUE* argv, VALUE self);
    static VALUE CallAsConstructor(VALUE self, VALUE r_context, VALUE argv);

    inline Object(VALUE value) : Ref<v8::Object>(value) {}
    inline Object(v8::Isolate* isolate, v8::Handle<v8::Object> object) : Ref<v8::Object>(isolate, object) {}

    typedef MaybeLocal<Object, v8::Object> Maybe;

    operator VALUE();
  };

}

#endif
