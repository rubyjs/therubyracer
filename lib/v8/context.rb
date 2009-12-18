module V8
  # This doesn't do anything at the moment. But the ruby interface will go here
  # The native interface is under the V8::C module.
  class Context    
    def initialize
      @native = C::Context.new
    end
    
    def open(&block)
      @native.open do
        block.call(self).tap do |result|
          # puts ERB::Util.h(result)
          raise JavascriptError.new(result) if result.kind_of?(C::Message)
        end
      end if block_given?
    end
    
    def eval(javascript)
      source = V8::C::String.new(javascript.to_s)
      script = V8::C::Script.new(source)
      To.ruby(script.Run())
    end
        
    def evaluate(*args)
      self.eval(*args)
    end
    
    def []=(key, value)
      value.tap do 
        @native.Global().tap do |scope|
          scope.Set(key.to_s, To.v8(value))
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
    
  end
end