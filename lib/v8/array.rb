class V8::Array < V8::Object
  def each
    @context.enter do
      0.upto(@native.Length() - 1) do |i|
        yield @context.to_ruby(@native.Get(i))
      end
    end
  end

  def length
    @native.Length()
  end
end