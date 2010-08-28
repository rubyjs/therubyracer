module V8
  class Function < V8::Object
    
    def methodcall(thisObject, *args)
      err = nil
      return_value = nil
      C::TryCatch.try do |try|
        @context.enter do
          this = To.v8(thisObject)
          return_value = To.rb(@native.Call(this, To.v8(args)))
          err = JSError.new(try) if try.HasCaught()
        end
      end
      raise err if err
      return return_value
    end
    
    def call(*args)
      self.methodcall(@context.Global(), *args)
    end
    
    def new(*args)
      @context.enter do
        To.rb(@native.NewInstance(To.v8(args)))
      end
    end

    # def self.rubyprotect(&blk)
    #   To.v8(rubyprotect2(&blk))
    # end
    # 
    # def self.rubyprotect2
    #   begin
    #     yield
    #   rescue Exception => e
    #     case e
    #     when SystemExit, NoMemoryError
    #       raise e
    #     else
    #       error = V8::C::Exception::Error(V8::C::String::New(e.message))
    #       error.SetHiddenValue("TheRubyRacer::Cause", C::External::New(e))
    #       V8::C::ThrowException(error)
    #     end
    #   end
    # end
    # 
    # def self.rubycall(rubycode, *args, &block)
    #   rubyprotect do
    #     rubycode.call(*args, &block)
    #   end
    # end
    # 
    # def self.rubysend(obj, message, *args, &block)
    #   rubyprotect do
    #     obj.send(message, *args, &block)
    #   end
    # end
  end
end