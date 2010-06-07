module V8
  class Function < V8::Object
    
    def methodcall(thisObject, *args)
      err = nil
      return_value = nil
      C::TryCatch.try do |try|
        @context.enter do
          this = To.v8(thisObject)
          return_value = To.rb(@native.Call(this, To.v8(args)))
          err = JavascriptError.new(try) if try.HasCaught()
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

    def self.rubycall(rubycode, *args)
      begin
        To.v8(rubycode.call(*args))
      rescue StandardError => e
        V8::C::ThrowException(V8::C::Exception::Error(V8::C::String::New(e.message)))
      end
    end

  end
end