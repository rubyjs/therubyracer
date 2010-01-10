
module V8
  class Object
    include Enumerable
    
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
    
    def each
      for prop in @native.GetPropertyNames()
        yield prop, self[prop]
      end
    end
  end
end