
module V8
  class Object
    include Enumerable
    
    def initialize(native, context = nil)
      @native = native
      @context = context || C::Context::GetEntered()
      raise ScriptError, "V8::Object.new called without an open V8 context" unless @context
    end
    
    def [](key)
      @context.enter do
        To.ruby(@native.Get(To.v8(key)))        
      end
    end
    
    def []=(key, value)
      value.tap do
        @context.enter do
          @native.Set(To.v8(key), To.v8(value))
        end
      end
    end
    
    def to_s
      @context.enter do
        To.rb(@native.ToString())
      end
    end
    
    def each
      for prop in To.rb(@native.GetPropertyNames())
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