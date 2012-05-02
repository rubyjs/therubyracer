module V8
  class Context

    def initialize
      @native = V8::C::Context::New()
    end

    def eval(source, filename = '<eval>', line = 1)
      @native.Enter()
      script = V8::C::Script::New(source.to_s, filename.to_s)
      script.Run()
    ensure
      @native.Exit()
    end
  end
end