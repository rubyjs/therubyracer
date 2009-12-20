
module V8
  class Object
    def initialize(native)
      @native = native
    end
    
    def [](key)
      To.ruby(@native.Get(key))
    end
  end
end