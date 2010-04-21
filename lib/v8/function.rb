module V8
  class Function < V8::Object
    
    def call(thisObject, *args)
      To.ruby(@native.Call(To.v8(thisObject), *args.map {|a| To.v8(a)}))
    end
    
  end
end