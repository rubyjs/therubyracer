// -*- mode: c++ -*-
#ifndef RR_OBJECT_TEMPLATE_H
#define RR_OBJECT_TEMPLATE_H

namespace rr {
  class ObjectTemplate : public Ref<v8::ObjectTemplate> {
  public:
    ObjectTemplate(VALUE self) : Ref<v8::ObjectTemplate>(self) {}
    ObjectTemplate(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> tmpl) :
      Ref<v8::ObjectTemplate>(isolate, tmpl) {}

    inline static void Init() {
      ClassBuilder("ObjectTemplate", Template::Class).
        defineSingletonMethod("New", &New).
        defineMethod("NewInstance", &NewInstance).
        defineMethod("SetAccessor", &SetAccessor).
        defineMethod("SetNamedPropertyHandler", &SetNamedPropertyHandler).
        defineMethod("SetIndexedPropertyHandler", &SetIndexedPropertyHandler).
        defineMethod("SetCallAsFunctionHandler", &SetCallAsFunctionHandler).
        defineMethod("InternalFieldCount", &InternalFieldCount).
        defineMethod("SetInternalFieldCount", &SetInternalFieldCount).
        store(&Class);
    }

    static VALUE New(int argc, VALUE argv[], VALUE self) {
      VALUE r_isolate, r_constructor;
      rb_scan_args(argc, argv, "11", &r_isolate, &r_constructor);
      Isolate isolate(r_isolate);
      Locker lock(isolate);

      return ObjectTemplate(isolate, v8::ObjectTemplate::New(isolate));
    }

    static VALUE NewInstance(VALUE self, VALUE r_context) {
      ObjectTemplate t(self);
      Context context(r_context);
      Isolate isolate(context.getIsolate());
      Locker lock(isolate);

      return Object::Maybe(isolate, t->NewInstance(context));
    }

    static VALUE InternalFieldCount(VALUE self) {
      ObjectTemplate t(self);
      Locker lock(t);

      return INT2NUM(t->InternalFieldCount());
    }

    static VALUE SetInternalFieldCount(VALUE self, VALUE value) {
      ObjectTemplate t(self);
      Locker lock(t);

      t->SetInternalFieldCount(NUM2INT(value));

      return Qnil;
    }

    static VALUE SetAccessor(int argc, VALUE argv[], VALUE self) {
      VALUE r_name, r_getter, r_setter, r_data, r_settings, r_attribute, r_signature;
      rb_scan_args(
        argc, argv, "25",
        &r_name, &r_getter, &r_setter, &r_data, &r_settings, &r_attribute, &r_signature
      );

      ObjectTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      PropertyCallback callback(isolate, r_getter, r_setter, r_data);

      t->SetAccessor(
        *Name(r_name),
        callback,
        callback,
        callback,
        Enum<v8::AccessControl>(r_settings, v8::DEFAULT),
        Enum<v8::PropertyAttribute>(r_attribute, v8::None),
        AccessorSignature(r_signature)
      );

      return Qnil;
    }

    static VALUE SetNamedPropertyHandler(int argc, VALUE argv[], VALUE self) {
      VALUE r_getter, r_setter, r_query, r_deleter, r_enumerator, r_data;
      rb_scan_args(
        argc, argv, "15",
        &r_getter, &r_setter, &r_query, &r_deleter, &r_enumerator, &r_data
      );

      ObjectTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      PropertyCallback callback(isolate, r_getter, r_setter, r_query, r_deleter, r_enumerator, r_data);

      t->SetNamedPropertyHandler(callback, callback, callback, callback, callback, callback);

      return Qnil;
    }

    static VALUE SetIndexedPropertyHandler(int argc, VALUE argv[], VALUE self) {
      VALUE r_getter, r_setter, r_query, r_deleter, r_enumerator, r_data;
      rb_scan_args(
        argc, argv, "15",
        &r_getter, &r_setter, &r_query, &r_deleter, &r_enumerator, &r_data
      );

      ObjectTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      PropertyCallback callback(isolate, r_getter, r_setter, r_query, r_deleter, r_enumerator, r_data);

      t->SetIndexedPropertyHandler(callback, callback, callback, callback, callback, callback);

      return Qnil;
    }

    static VALUE SetCallAsFunctionHandler(VALUE self, VALUE r_callback, VALUE r_data) {
      ObjectTemplate t(self);
      Isolate isolate(t.getIsolate());
      Locker lock(isolate);

      FunctionCallback callback(isolate, r_callback, r_data);

      t->SetCallAsFunctionHandler(callback, callback);

      return Qnil;
    }
  };
}


#endif /* RR_OBJECT_TEMPLATE_H */
