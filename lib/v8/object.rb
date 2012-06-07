class V8::Object
  include Enumerable
  attr_reader :native

  def initialize(native)
    @native = native
    @context = V8::Context.current
  end

  def [](key)
    @context.enter do
      @context.to_ruby @native.Get(@context.to_v8(key))
    end
  end

  def []=(key, value)
    @context.enter do
      @native.Set(@context.to_v8(key), @context.to_v8(value))
    end
    return value
  end

  def each
    @context.enter do
      names = @native.GetPropertyNames()
      0.upto(@native.Length() - 1) do |i|
        name = names.Get(i)
        yield @context.to_ruby(name), @context.to_ruby(@native.Get(name))
      end
    end
  end
end