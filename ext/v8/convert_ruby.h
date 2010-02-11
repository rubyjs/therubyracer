#ifndef __convert_ruby_h__
#define __convert_ruby_h__

#include <ruby.h>
#include <string>

/**
 * A RubyValueSource takes a Destination class and a return
 * type as a template argument, it converts a Ruby Value to
 * the appropriate intermediate type and feed it to an 
 * instance Destination type.  
 * 
 * The destination type should have a default constructor, 
 * and provide the methods detailed in data_conversion.txt
 * \tparam DEST destination type for the converted data
 * \tparam RET is the return type of T's methods
 */

template<class DEST, class RET> class RubyValueSource {

    /**
    * An instance of the destination class
    */
    DEST dest;

    public:
    RubyValueSource() {}
    ~RubyValueSource() {}


    bool operator() (VALUE& value, RET& result) {
        switch (TYPE(value)) {
        case T_FIXNUM:
          result = dest.pushInt(FIX2INT(value));
          return true;
        case T_FLOAT:
          result = dest.pushDouble(NUM2DBL(value));
          return true;
        case T_STRING:
          result = convertString(value);
          return true;
        case T_NIL:
          result = dest.pushNull();
          return true;
        case T_TRUE:
          result = dest.pushBool(true);
          return true;
        case T_FALSE:
          result = dest.pushBool(false);
          return true;
        }
        return false;
    }
    
private:
    RET convertString(VALUE& value) {
        std::string stringValue(RSTRING_PTR(value));
        return dest.pushString(stringValue);
    }
};

/**
 * A RubyValueDest class receives on of the types defined in
 * data_conversion.txt, and converts it to the appropriate
 * Ruby VALUE.
 */
class RubyValueDest {

    public:
    RubyValueDest();
    ~RubyValueDest();

    VALUE pushString(const std::string& value) {
        return rb_str_new2(value.c_str());
    }

    VALUE pushInt(int64_t value) {
        return INT2FIX(value);
    }

    VALUE pushDouble(double value) {
        return rb_float_new(value);
    }

    VALUE pushBool(bool value) {
        return value ? Qtrue : Qfalse;
    }

    VALUE pushNull() {
        return Qnil;
    }

    VALUE pushUndefined() {
        return Qnil;
    }
};


#endif
