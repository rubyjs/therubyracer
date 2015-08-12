##
# An extensible conversion mechanism for converting objects to and
# from their V8 representations.
#
# The Ruby Racer has two levels of representation for JavaScript
# objects: the low-level C++ objects which are just thin wrappers
# native counterparts. These are the objects in the `V8::C::*`
# namespace. It also has high-level Ruby objects which can
# either be instances of `V8::Object`, `V8::Date`, or just plain Ruby
# objects that have representations in the JavaScript runtime.
#
# The conversion object held by the context captures this transition
# from one object type to the other. It is implemented as a "middleware"
# stack where at the base is the `Fundamental` conversion which does
# basic conversion and identity mapping.
#
# In order to "extend" or override the conversion mechanism, you can
# extend this object to add behaviors. For example, the following
# extension will add a `__fromRuby__` property to every ruby object
# that is embedded into this context.
#
#   module TagRubyObjects
#     def to_v8(context, ruby_object)
#       super.tap do |v8_object|
#         v8_object.Set(V8::C::String::NewFromUtf8("__fromRuby__"), V8::C::Boolean::New(true))
#       end
#     end
#   end
#   context.conversion.extend TagRubyObjects
#
# @see V8::Conversion::Fundamental for the basic conversion operation.
class V8::Conversion
  include Fundamental


  ##
  # Convert a low-level instance of `V8::C::Value` or one of its
  # subclasses into a Ruby object.
  #
  # The `Fundamental` conversion will call `v8_object.to_ruby`, but
  # any number of middlewares can be inserted between then.
  #
  # @param [V8::C::Value] the object to convert
  # @return [Object] the corresponding Ruby value
  def to_ruby(v8_object)
    super v8_object
  end

  ##
  # Convert a Ruby Object into a low-level `V8::C::Value` or one of
  # its subclasses. Note here that things like `V8::Object` are
  # considered Ruby objects and *not* V8 objects. So, for example, the
  # fundamental conversion for `V8::Object` will return a
  # `V8::C::Object`
  #
  # The `Fundamental` conversion will call
  # `ruby_object.to_v8(context)` optionally storing the result in an
  # identity map in the case where the result is a `V8::C::Object`
  #
  # @param [V8::Context] the Ruby context in the conversion happens
  # @param [Object] Ruby object to convert
  # @return [V8::C::Value] the v8 representation
  def to_v8(context, ruby_object)
    super context, ruby_object
  end
end


##
# The folowing represent the default conversions from instances of
# `V8::C::Value` into their Ruby counterparts.
module V8::C
  class String
    def to_ruby
      self.Utf8Value()
    end
  end

  class Number
    def to_ruby
      self.Value()
    end
  end

  class Undefined
    def to_ruby
      nil
    end
  end

  class Boolean
    def to_ruby
      IsTrue()
    end
  end

  class Object
    def to_ruby
      ::V8::Object.new self
    end
  end

  class Date
    def to_ruby
      ::V8::Date.new self
    end
  end

  class Function
    def to_ruby
      ::V8::Function.new self
    end
  end

  class Value
    def to_v8(context)
      self
    end
  end
end

##
# The following are the default conversions from Ruby objects into
# low-level C++ objects.
class String
  def to_v8(context)
    V8::C::String::NewFromUtf8(context.isolate.native, self)
  end
end

class Fixnum
  def to_v8(context)
    V8::C::Integer::New(context.isolate.native, self)
  end
end

class Symbol
  def to_v8(context)
    isolate = context.isolate.native
    V8::C::Symbol::For(context.isolate.native, V8::C::String::NewFromUtf8(isolate, to_s))
  end
end

class Object
  def to_v8(context)
    V8::C::Object::New(context.isolate.native)
  end
end

class Proc
  def to_v8(context)
    isolate = context.isolate.native
    callback = lambda do |info|
      args = []
      arity = info.Length()
      if self.arity > -1
        arity = self.arity
      end
      arity.times do |i|
        args << context.to_ruby(info[i])
      end
      result = context.to_v8 self.call(*args)
      info.GetReturnValue().Set(result)
    end
    V8::C::Function::New(isolate, callback)
  end
end
