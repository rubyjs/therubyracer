// -*- mode: c++ -*-
#ifndef RR_DATE_H
#define RR_DATE_H

namespace rr {
  class Date : public Ref<v8::Date> {
  public:
    Date(v8::Isolate* isolate, v8::Local<v8::Date> date) :
      Ref<v8::Date>(isolate, date) {}

    static void Init() {
      ClassBuilder("Date", Object::Class).
        store(&Class);
    };
  };
}

#endif /* RR_DATE_H */
