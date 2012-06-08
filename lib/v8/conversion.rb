module V8::Conversion
  def to_ruby(v8_object)
    v8_cache.get(v8_object) do
      v8_object.respond_to?(:to_ruby) ? v8_object.to_ruby : v8_object
    end
  end

  def to_v8(ruby_object)
    ruby_object.respond_to?(:to_v8) ? ruby_object.to_v8 : V8::C::Object::New()
  end

  def v8_cache
    @v8_cache ||= Cache.new
  end


  class Cache
    def initialize
      @storage = {}
    end

    def get(v8_object)
      if v8_object.is_a?(V8::C::Object)
        weakref = @storage[v8_object.GetIdentityHash()]
        if weakref && weakref.weakref_alive?
          weakref.__getobj__
        else
          @storage[v8_object.GetIdentityHash()] = WeakRef.new(yield)
        end
      else
        yield
      end
    end
  end
end

class Numeric
  def to_v8
    self
  end
end

class V8::C::String
  def to_ruby
    self.Utf8Value()
  end
end

class String
  def to_v8
    V8::C::String::New(self)
  end
end

class Symbol
  def to_v8
    V8::C::String::NewSymbol(to_s)
  end
end

class V8::C::Date
  def to_ruby
    Time.at(self.NumberValue() / 1000)
  end
end

class Time
  def to_v8
    V8::C::Date::New(to_f * 1000)
  end
end

class V8::C::Object
  def to_ruby
    V8::Object.new(self)
  end
end

class V8::Object
  def to_v8
    self.native
  end
end

class V8::C::Array
  def to_ruby
    V8::Array.new(self)
  end
end

class Array
  def to_v8
    array = V8::Array.new(length)
    each_with_index do |item, i|
      array[i] = item
    end
    return array.to_v8
  end
end

class Hash
  def to_v8
    object = V8::Object.new
    each do |key, value|
      object[key] = value
    end
    return object.to_v8
  end
end