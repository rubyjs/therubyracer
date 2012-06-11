
class V8::Conversion
  include Fundamental
  include Identity

  def to_ruby(v8_object)
    super v8_object
  end

  def to_v8(ruby_object)
    super ruby_object
  end
end

for type in [Numeric, Object, Array, Hash, String, Symbol, Time, Proc] do
  type.class_eval do
    include V8::Conversion.const_get(name)
  end
end

class Numeric
  include V8::Conversion::Numeric
end

class Object
  include V8::Conversion::Object
end

class Array
  include V8::Conversion::Array
end

class Hash
  include V8::Conversion::Hash
end

class String
  include V8::Conversion::String
end

class Symbol
  include V8::Conversion::Symbol
end

class Time
  include V8::Conversion::Time
end

class Proc
  include V8::Conversion::Proc
end

class V8::C::Object
  include V8::Conversion::NativeObject
end

class V8::C::Array
  include V8::Conversion::NativeArray
end

class V8::C::String
  include V8::Conversion::NativeString
end

class V8::C::Date
  include V8::Conversion::NativeDate
end

