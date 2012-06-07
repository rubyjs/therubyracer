module V8::Conversion
  def to_ruby(v8_object)
    v8_object.respond_to?(:to_ruby) ? v8_object.to_ruby : v8_object
  end

  def to_v8(ruby_object)
    ruby_object.respond_to?(:to_v8) ? ruby_object.to_v8 : case ruby_object
    when Numeric then ruby_object
    else
      V8::C::Object::New()
    end
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

