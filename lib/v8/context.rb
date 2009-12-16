module V8
  # This doesn't do anything at the moment. But the ruby interface will go here
  # The native interface is under the V8::C module.
  class Context    
    def initialize
      @native = C::Context.new
    end
    
    def open(&block)
      @native.open do
        block.call(self) if block
      end if block_given?
    end
    
    def eval(javascript)
      self.open do
        source = V8::C::String.new(javascript.to_s)
        script = V8::C::Script.new(source)
        script.Run()
      end
    end
        
    def evaluate(*args)
      self.eval(*args)
    end
    
    def self.open(&block)
      new.open(&block)
    end
  end
end