#ifndef RR_OBJECT
#define RR_OBJECT

namespace rr {

  class Object : public Ref<v8::Object> {
  public:
    static void Init();
    static VALUE New(VALUE self, VALUE isolate);
    static VALUE Set(VALUE self, VALUE key, VALUE value);
    // static VALUE ForceSet(VALUE self, VALUE key, VALUE value);
    static VALUE Get(VALUE self, VALUE key);
    // static VALUE GetPropertyAttributes(VALUE self, VALUE key);
    static VALUE Has(VALUE self, VALUE context, VALUE key);
    // static VALUE Delete(VALUE self, VALUE key);
    // static VALUE ForceDelete(VALUE self, VALUE key);
    // static VALUE SetAccessor(int argc, VALUE* argv, VALUE self);
    // static VALUE GetPropertyNames(VALUE self);
    // static VALUE GetOwnPropertyNames(VALUE self);
    // static VALUE GetPrototype(VALUE self);
    // static VALUE SetPrototype(VALUE self, VALUE prototype);
    // static VALUE FindInstanceInPrototypeChain(VALUE self, VALUE impl);
    // static VALUE ObjectProtoToString(VALUE self);
    // static VALUE GetConstructorName(VALUE self);
    // static VALUE InternalFieldCount(VALUE self);
    // static VALUE GetInternalField(VALUE self, VALUE idx);
    // static VALUE SetInternalField(VALUE self, VALUE idx, VALUE value);
    // static VALUE HasOwnProperty(VALUE self, VALUE key);
    // static VALUE HasRealNamedProperty(VALUE self, VALUE key);
    // static VALUE HasRealIndexedProperty(VALUE self, VALUE idx);
    // static VALUE HasRealNamedCallbackProperty(VALUE self, VALUE key);
    // static VALUE GetRealNamedPropertyInPrototypeChain(VALUE self, VALUE key);
    // static VALUE GetRealNamedProperty(VALUE self, VALUE key);
    // static VALUE HasNamedLookupInterceptor(VALUE self);
    // static VALUE HasIndexedLookupInterceptor(VALUE self);
    // static VALUE TurnOnAccessCheck(VALUE self);
    // static VALUE GetIdentityHash(VALUE self);
    // static VALUE SetHiddenValue(VALUE self, VALUE key, VALUE value);
    // static VALUE GetHiddenValue(VALUE self, VALUE key);
    // static VALUE DeleteHiddenValue(VALUE self, VALUE key);
    // static VALUE IsDirty(VALUE self);
    // static VALUE Clone(VALUE self);
    // static VALUE CreationContext(VALUE self);
    // static VALUE SetIndexedPropertiesToPixelData(VALUE self, VALUE data, VALUE length);
    // static VALUE GetIndexedPropertiesPixelData(VALUE self);
    // static VALUE HasIndexedPropertiesInPixelData(VALUE self);
    // static VALUE GetIndexedPropertiesPixelDataLength(VALUE self);
    // static VALUE SetIndexedPropertiesToExternalArrayData(VALUE self);
    // static VALUE HasIndexedPropertiesInExternalArrayData(VALUE self);
    // static VALUE GetIndexedPropertiesExternalArrayData(VALUE self);
    // static VALUE GetIndexedPropertiesExternalArrayDataType(VALUE self);
    // static VALUE GetIndexedPropertiesExternalArrayDataLength(VALUE self);
    // static VALUE IsCallable(VALUE self);
    // static VALUE CallAsFunction(VALUE self, VALUE recv, VALUE argv);
    // static VALUE CallAsConstructor(VALUE self, VALUE argv);

    inline Object(VALUE value) : Ref<v8::Object>(value) {}
    inline Object(v8::Isolate* isolate, v8::Handle<v8::Object> object) : Ref<v8::Object>(isolate, object) {}

    virtual operator VALUE();

  protected:
    VALUE downcast();
  };

}

#endif
