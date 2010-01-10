
module V8
  class Object
    def initialize(native)
      @native = native
    end
    
    def [](key)
      To.ruby(@native.Get(key.to_s))
    end
    
    def []=(key, value)
      value.tap do
        @native.Set(key.to_s, value)
      end
    end
  end
end