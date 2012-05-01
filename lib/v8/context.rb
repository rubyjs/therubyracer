module V8
  class Context

    def initialize
      @native = V8::C::Context::New()
    end

    def eval(source, filename = '<eval>', line = 1)
      nil
    end
  end
end