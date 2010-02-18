
module V8
  class Object
    include Enumerable
    
    def initialize(native)
      @native = native
    end
    
    def [](key)
      @native.context.open do
        To.ruby(@native.Get(key.to_s))        
      end
    end
    
    def []=(key, value)
      value.tap do
        @native.context.open do
          @native.Set(key.to_s, value)
        end
      end
    end
    
    def to_s
      @native.context.open do
        @native.ToString()
      end
    end
    
    def each
      for prop in @native.GetPropertyNames()
        yield prop, self[prop]
      end
    end
  end
end

class Object  
  def eval_js(javascript)
    V8::Context.open(:with => self) do |cxt|
      cxt.eval(javascript)
    end
  end
end