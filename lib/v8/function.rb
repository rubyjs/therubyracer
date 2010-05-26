module V8
  class Function < V8::Object
    
    def call(thisObject, *args)
      err = nil
      return_value = nil
      C::TryCatch.try do |try|
        @context.enter do
          this = To.v8(thisObject)
          return_value = To.ruby(@native.Call(this, *args.map {|a| To.v8(a)}))
          err = JavascriptError.new(try) if try.HasCaught()
        end
      end
      raise err if err
      return return_value
    end
  end
end