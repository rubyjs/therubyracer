class V8::Conversion
  module Class
    def to_v8
      v8_constructor.GetFunction()
    end

    def v8_constructor
      unless @v8_constructor && @v8_constructor.weakref_alive?
        template = V8::C::FunctionTemplate::New()
        # template.SetCallHandler(InvocationHandler.new, V8::C::External::New(self))
        @v8_constructor = WeakRef.new(template)
      end
      return @v8_constructor.__getobj__
    end

  end
end