module V8
  class Function < V8::Object
    
    def call(thisObject, *args)
      this = To.v8(thisObject)
      To.ruby(@native.Call(this, *args.map {|a| To.v8(a)}))
    end
    
  end
end