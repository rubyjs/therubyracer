class V8::Array < V8::Object
  def each
    @context.enter do
      for i in 0..(@native.Length() - 1)
        yield @context.to_ruby(@native.Get(i))
      end
    end
  end

  def length
    @native.Length()
  end
end