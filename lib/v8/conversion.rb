class V8::Conversion
  include Fundamental
  # include Identity

  def to_ruby(v8_object)
    super v8_object
  end

  def to_v8(context, ruby_object)
    super context, ruby_object
  end
end

module V8::C
  class String
    alias_method :to_ruby, :Utf8Value
  end

  class Number
    alias_method :to_ruby, :Value
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
end

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

# for type in [TrueClass, FalseClass, NilClass, Float] do
#   type.class_eval do
#     include V8::Conversion::Primitive
#   end
# end

# for type in [Class, Object, Array, Hash, String, Symbol, Time, Proc, Method, Fixnum] do
#   type.class_eval do
#     include V8::Conversion.const_get(type.name)
#   end
# end

# class UnboundMethod
#   include V8::Conversion::Method
# end

# for type in [:Object, :String, :Date] do
#   V8::C::const_get(type).class_eval do
#     include V8::Conversion::const_get("Native#{type}")
#   end
# end
