
module V8
  class Array < V8::Object
    
    def each
      for i in 0..(@native.Length() - 1)
        yield To.ruby(@native.Get(i))
      end
    end
  end
end