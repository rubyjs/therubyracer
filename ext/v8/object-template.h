// -*- mode: c++ -*-
#ifndef RR_OBJECT_TEMPLATE_H
#define RR_OBJECT_TEMPLATE_H

namespace rr {
  class ObjectTemplate : Ref<v8::Template> {
  public:
    ObjectTemplate(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> tmpl) :
      Ref<v8::Template>(isolate, tmpl) {}
    inline static void Init() {
      ClassBuilder("ObjectTemplate", Template::Class).
        defineSingletonMethod("New", &New).
        store(&Class);
    }

    static VALUE New(int argc, VALUE argv[], VALUE self) {
      VALUE r_isolate, r_constructor;
      rb_scan_args(argc, argv, "11", &r_isolate, &r_constructor);
      Isolate isolate(r_isolate);
      Locker lock(isolate);
      return ObjectTemplate(isolate, v8::ObjectTemplate::New(isolate));
    }

    static VALUE NewInstance(VALUE self , VALUE context) {

    }
  };
}


#endif /* RR_OBJECT_TEMPLATE_H */
