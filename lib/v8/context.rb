require 'stringio'

module V8
  class Context    
    attr_reader :native, :scope
    def initialize(opts = {})
      @native = C::Context::New(opts[:with])
      @native.enter do
        @scope = To.rb(@native.Global())
      end
      yield(self) if block_given?
    end
    
    def eval(javascript, filename = "<eval>", line = 1)
      if IO === javascript || StringIO === javascript
        javascript = javascript.read()
      end
      err = nil
      value = nil
      C::TryCatch.try do |try|
        @native.enter do
          script = C::Script::Compile(To.v8(javascript.to_s), To.v8(filename.to_s))
          if try.HasCaught()
            err = JavascriptError.new(try)
          else
            result = script.Run()
            if try.HasCaught()
              err = JavascriptError.new(try)
            else
              value = To.ruby(result)
            end
          end
        end
      end
      raise err if err
      return value
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
      @scope[key]
    end
    
    def []=(key, value)
      @scope[key] = value
    end

    def self.eval(source)
      new.eval(source)
    end
    
    def V8.eval(*args)
      V8::Context.eval(*args)
    end
  end
    
  class JavascriptError < StandardError
    attr_reader :source_name, :source_line, :line_number, :javascript_stacktrace
    
    def initialize(try)
      msg = try.Message()
      @source_name = To.ruby(msg.GetScriptResourceName())
      @source_line = To.ruby(msg.GetSourceLine())
      @line_number = To.ruby(msg.GetLineNumber())
      @javascript_stacktrace = To.ruby(try.StackTrace())
      super("#{To.ruby(msg.Get())}: #{@source_name}:#{@line_number}")
    end
    
    def self.check(try)
      raise JavascriptError.new(try) if try.HasCaught()
    end

  end
  
  module C
    class Context
      def enter
        if block_given?
          if IsEntered()
            yield(self)
          else
            Enter()
            begin
              yield(self)
            ensure
              Exit()
            end
          end
        end
      end
    end
  end
end