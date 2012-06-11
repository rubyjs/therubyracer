class V8::Object
  include Enumerable
  attr_reader :native
  alias_method :to_v8, :native

  def initialize(native = nil)
    @context = V8::Context.current or fail "tried to initialize a #{self.class} without being in an entered V8::Context"
    @native = native || V8::C::Object::New()
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
      0.upto(names.Length() - 1) do |i|
        name = names.Get(i)
        yield @context.to_ruby(name), @context.to_ruby(@native.Get(name))
      end
    end
  end

  def to_s
    @context.enter do
      "#{self.class}#{@native.ToString().Utf8Value()}"
    end
  end
end