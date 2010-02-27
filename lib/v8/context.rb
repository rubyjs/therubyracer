require 'stringio'

module V8  
  class Context    
    def initialize(opts = {})      
      @native = C::Context.new(opts[:with])
    end
    
    def open(&block)
      if block_given?
        unless @native == C::Context::GetCurrent()
          @native.open do
            block.call(self)
          end
        else
          block.call(self)
        end
      end
    end
    
    def eval(javascript, sourcename = '<eval>', line = 1)
      if IO === javascript || StringIO === javascript
        javascript = javascript.read()
      end
      @native.open do        
        @native.eval(javascript, sourcename).tap do |result|
          raise JavascriptError.new(result) if result.kind_of?(C::Message)
          return To.ruby(result)
        end
      end
    end
        
    def evaluate(*args)
      self.eval(*args)
    end
    
    def load(filename)
      File.open(filename) do |file|
        evaluate file, filename, 1
      end      
    end
    
    def [](key)
      open do
        To.ruby(@native.Global().Get(key.to_s))
      end
    end
    
    def []=(key, value)
      value.tap do 
        open do
          @native.Global().tap do |scope|
            scope.Set(key.to_s, value)
          end
        end
      end
    end
    
    def self.open(opts = {}, &block)
      new(opts).open(&block)
    end
    
    def self.eval(source)
      new.eval(source)
    end
    
    def V8.eval(*args)
      V8::Context.eval(*args)
    end
  end
  
  class ContextError < StandardError
    def initialize(caller_name)
      super("tried to call method '#{caller_name} without an open context")
    end
    def self.check_open(caller_name)
      raise new(caller_name) unless C::Context::InContext()
    end
  end
  class JavascriptError < StandardError
    def initialize(v8_message)
      super("#{v8_message.Get()}: #{v8_message.GetScriptResourceName()}:#{v8_message.GetLineNumber()}")
      @native = v8_message
    end

    def source_name
      @native.GetScriptResourceName()      
    end
    
    def source_line
      @native.GetSourceLine()
    end
    
    def line_number
      @native.GetLineNumber()
    end
    
  end
  class RunawayScriptError < ContextError
  end
end