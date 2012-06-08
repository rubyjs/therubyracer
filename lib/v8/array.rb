class V8::Array < V8::Object

  def initialize(native_or_length = nil)
    if native_or_length.is_a?(Numeric)
      super V8::C::Array::New(native.to_i)
    elsif native_or_length.is_a?(V8::C::Array)
      super native_or_length
    else
      super V8::C::Array::New()
    end
  end

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