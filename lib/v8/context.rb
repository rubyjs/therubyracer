require 'stringio'

module V8
  class Context    
    attr_reader :native, :scope, :access
    def initialize(opts = {})
      @access = Access.new
      @to = Portal.new(self, @access)
      @native = opts[:with] ? C::Context::New(@to.rubytemplate) : C::Context::New()
      @native.enter do
        @global = @native.Global()
        @scope = @to.rb(@global)
        @global.SetHiddenValue(C::String::New("TheRubyRacer::RubyObject"), C::External::New(opts[:with])) if opts[:with]
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
          script = C::Script::Compile(@to.v8(javascript.to_s), @to.v8(filename.to_s))
          if try.HasCaught()
            err = JSError.new(try, @to)
          else
            result = script.Run()
            if try.HasCaught()
              err = JSError.new(try, @to)
            else
              value = @to.rb(result)
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