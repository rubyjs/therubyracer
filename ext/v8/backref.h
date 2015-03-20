#ifndef RR_BACKREF
#define RR_BACKREF

namespace rr {

  class Backref {
  public:
    static void Init();

    Backref(VALUE value);

    virtual ~Backref();

    VALUE get();
    VALUE set(VALUE value);

    v8::Handle<v8::External> toExternal();

    static void release(const v8::WeakCallbackData<v8::External, Backref>& data);
  private:
    VALUE storage;
    static VALUE Storage;
    static ID _new;
    static ID object;
  };

}

#endif
