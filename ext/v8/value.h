#ifndef RR_VALUE
#define RR_VALUE

namespace rr {

  class Value : public Ref<v8::Value> {
  public:
    static void Init();

    static VALUE IsUndefined(VALUE self);
    static VALUE IsNull(VALUE self);
    static VALUE IsTrue(VALUE self);
    static VALUE IsFalse(VALUE self);
    static VALUE IsString(VALUE self);
    // static VALUE IsFunction(VALUE self);
    // static VALUE IsArray(VALUE self);
    static VALUE IsObject(VALUE self);
    static VALUE IsBoolean(VALUE self);
    // static VALUE IsNumber(VALUE self);
    static VALUE IsExternal(VALUE self);
    static VALUE IsInt32(VALUE self);
    static VALUE IsUint32(VALUE self);
    // static VALUE IsDate(VALUE self);
    // static VALUE IsBooleanObject(VALUE self);
    // static VALUE IsNumberObject(VALUE self);
    // static VALUE IsStringObject(VALUE self);
    // static VALUE IsNativeError(VALUE self);
    // static VALUE IsRegExp(VALUE self);
    static VALUE ToString(VALUE self);
    // static VALUE ToDetailString(VALUE self);
    // static VALUE ToObject(VALUE self);
    // static VALUE BooleanValue(VALUE self);
    // static VALUE NumberValue(VALUE self);
    // static VALUE IntegerValue(VALUE self);
    // static VALUE Uint32Value(VALUE self);
    // static VALUE Int32Value(VALUE self);

    static VALUE Equals(VALUE self, VALUE other);
    static VALUE StrictEquals(VALUE self, VALUE other);

    inline Value(VALUE value) : Ref<v8::Value>(value) {}
    inline Value(v8::Handle<v8::Value> value) : Ref<v8::Value>(value) {}

    virtual operator VALUE();
    virtual operator v8::Handle<v8::Value>() const;

    static VALUE Empty;
  };

}

#endif
