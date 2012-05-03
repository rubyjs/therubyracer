module V8
  class Context

    def initialize
      @native = V8::C::Context::New()
    end

    def eval(source, filename = '<eval>', line = 1)
      @native.Enter()
      source = V8::C::String::New(source.to_s)
      filename = V8::C::String::New(filename.to_s)
      script = V8::C::Script::New(source, filename)
      script.Run()
    ensure
      @native.Exit()
    end
  end
end