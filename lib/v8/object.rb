
module V8
  class Object
    def initialize(native)
      @native = native
    end
    
    def [](key)
      To.ruby(@native.Get(To.v8(key)))
    end
  end
end