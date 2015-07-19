// -*- mode: c++ -*-
#ifndef RR_VALUE
#define RR_VALUE

namespace rr {

  class Value : public Ref<v8::Value> {
  public:
    /**
     * A conversion class for down-thunking a MaybeLocal<v8::Value>
     * and returning it to Ruby as a `V8::C::Maybe`. If there is a
     * value present, then run it through the value conversion to get
     * the most specific subclass of Value:
     *
     *   return Value::Maybe(object->Get(cxt, key));
     */
    class Maybe : public rr::Maybe {
    public:
      Maybe(v8::Isolate* isolate, v8::MaybeLocal<v8::Value> maybe) {
        if (!maybe.IsEmpty()) {
          just(Value::handleToRubyObject(isolate, maybe.ToLocalChecked()));
        }
      }
    };

    static void Init();

    static VALUE IsUndefined(VALUE self);
    static VALUE IsNull(VALUE self);
    static VALUE IsTrue(VALUE self);
    static VALUE IsFalse(VALUE self);
    static VALUE IsString(VALUE self);
    static VALUE IsFunction(VALUE self);
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

    static VALUE ToRubyObject(VALUE self);
    static VALUE FromRubyObject(VALUE self, VALUE isolate, VALUE value);

    static VALUE Equals(VALUE self, VALUE other);
    static VALUE StrictEquals(VALUE self, VALUE other);

    inline Value(VALUE value) : Ref<v8::Value>(value) {}
    inline Value(v8::Isolate* isolate, v8::Handle<v8::Value> value) : Ref<v8::Value>(isolate, value) {}

    static VALUE handleToRubyObject(v8::Isolate* isolate, v8::Handle<v8::Value> value);
    static v8::Handle<v8::Value> rubyObjectToHandle(v8::Isolate* isolate, VALUE value);

    static std::vector< v8::Handle<v8::Value> > convertRubyArray(v8::Isolate* isolate, VALUE value);
  };

}

#endif
