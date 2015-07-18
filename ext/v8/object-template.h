// -*- mode: c++ -*-
#ifndef RR_OBJECT_TEMPLATE_H
#define RR_OBJECT_TEMPLATE_H

namespace rr {
  class ObjectTemplate : Ref<v8::ObjectTemplate> {
  public:
    ObjectTemplate(VALUE self) : Ref<v8::ObjectTemplate>(self) {}
    ObjectTemplate(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> tmpl) :
      Ref<v8::ObjectTemplate>(isolate, tmpl) {}
    inline static void Init() {
      ClassBuilder("ObjectTemplate", Template::Class).
        defineSingletonMethod("New", &New).
        defineMethod("NewInstance", &NewInstance).
        store(&Class);
    }

    static VALUE New(int argc, VALUE argv[], VALUE self) {
      VALUE r_isolate, r_constructor;
      rb_scan_args(argc, argv, "11", &r_isolate, &r_constructor);
      Isolate isolate(r_isolate);
      Locker lock(isolate);
      return ObjectTemplate(isolate, v8::ObjectTemplate::New(isolate));
    }

    static VALUE NewInstance(VALUE self , VALUE r_context) {
      ObjectTemplate t(self);
      Context context(r_context);
      Isolate isolate(context.getIsolate());
      Locker lock(isolate);
      v8::MaybeLocal<v8::Object> object(t->NewInstance());
      return Object::Maybe(isolate, ObjectTemplate(self)->NewInstance(context));
    }

  };
}


#endif /* RR_OBJECT_TEMPLATE_H */
