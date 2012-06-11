class V8::Conversion
  module Object
    def to_v8
      V8::C::Object::New()
    end

    def to_ruby
      self
    end
  end

  module NativeObject
    def to_ruby
      ::V8::Object.new(self)
    end
  end
end