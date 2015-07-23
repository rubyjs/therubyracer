module V8
  class Isolate
    attr_reader :native

    def initialize()
      @native = V8::C::Isolate::New()
    end
  end
end
