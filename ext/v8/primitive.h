#ifndef RR_PRIMITIVE
#define RR_PRIMITIVE

namespace rr {

  class Primitive: public Ref<v8::Primitive> {
  public:
    static void Init();

    inline Primitive(VALUE value) : Ref<v8::Primitive>(value) {}
    inline Primitive(v8::Isolate* isolate, v8::Handle<v8::Primitive> primitive) : Ref<v8::Primitive>(isolate, primitive) {}
  };

}

#endif
