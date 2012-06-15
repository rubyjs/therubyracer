class V8::Conversion
  module Object
    include V8::Util::Weakcell

    def to_v8
      context = V8::Context.current
      constructor = context.to_v8(self.class)
      object = constructor.NewInstance([V8::C::External::New(self)])
      return object
    end

    def to_ruby
      self
    end

    def eval_js(source, filename = '<eval>')
      V8::Context.new(:with => self).eval(source, filename)
    end
  end

  module NativeObject
    def to_ruby
      wrap = if IsArray()
        ::V8::Array
      elsif IsFunction()
        ::V8::Function
      else
        ::V8::Object
      end
      wrap.new(self)
    end

    def to_v8
      self
    end
  end
end