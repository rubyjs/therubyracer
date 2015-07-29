// -*- mode: c++ -*-
#ifndef RR_TEMPLATE_H
#define RR_TEMPLATE_H

namespace rr {
  class Template : public Ref<v8::Template> {
  public:
    Template(VALUE t) : Ref<v8::Template>(t) {}

    inline static void Init() {
      ClassBuilder("Template").
        defineMethod("Set", &Set).
        defineMethod("SetAccessorProperty", &SetAccessorProperty).
        store(&Class);
    }

    static VALUE Set(int argc, VALUE argv[], VALUE self) {
      VALUE name, value, r_attributes;
      rb_scan_args(argc, argv, "21", &name, &value, &r_attributes);
      v8::PropertyAttribute attributes(v8::PropertyAttribute::None);
      if (RTEST(r_attributes)) {
        attributes = (v8::PropertyAttribute)NUM2INT(r_attributes);
      }
      v8::Local<v8::Value> val = Value(value);
      Template(self)->Set(Name(name), val, attributes);
      return Qnil;
    }

    static VALUE SetAccessorProperty(int argc, VALUE argv[], VALUE self) {
      VALUE r_name, r_getter, setter, attributes, access;
      rb_scan_args(argc, argv, "14", &r_name, &r_getter, &setter, &attributes, &access);
      Template(self)->SetAccessorProperty(Name(r_name));
      return Qnil;
    }
  };
}


#endif /* RR_TEMPLATE_H */
