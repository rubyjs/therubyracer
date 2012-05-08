module V8
  class Context

    def initialize
      @native = V8::C::Context::New()
    end

    def eval(source, filename = '<eval>', line = 1)
      @native.Enter()
      V8::C::HandleScope() do
        source = V8::C::String::New(source.to_s)
        filename = V8::C::String::New(filename.to_s)
        script = V8::C::Script::New(source, filename)
        result = script.Run()
        result.kind_of?(V8::C::String) ? result.Utf8Value() : result
      end
    ensure
      @native.Exit()
    end

    def []=(key, value)
      V8::C::HandleScope() do
        @native.Global().Set(key, value)
      end
    end
    def [](key)
      V8::C::HandleScope() do
        @native.Global().Get(key)
      end
    end
  end
end