module V8
  class Context    
    def initialize
      @native = C::Context.new
    end
    
    def open(&block)
      @native.open do
        block.call(self)
      end if block_given?
    end
    
    def eval(javascript)
      @native.eval(javascript).tap do |result|
        raise JavascriptError.new(result) if result.kind_of?(C::Message)
        return To.ruby(result)
      end
    end
        
    def evaluate(*args)
      self.eval(*args)
    end
    
    def []=(key, value)
      value.tap do 
        @native.Global().tap do |scope|
          scope.Set(key.to_s, value)
        end
      end
    end
    
    def self.open(&block)
      new.open(&block)
    end    
  end
  
  class ContextError < StandardError
  end
  class JavascriptError < StandardError
    def initialize(v8_message)
      super(v8_message.Get())
    end
  end
  class RunawayScriptError < ContextError
  end
end