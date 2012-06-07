class V8::Object
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
end