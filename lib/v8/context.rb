module V8
  class Context
    def initialize
      @native = CC::Context::New()
    end
    def eval(*args)

    end
  end
end