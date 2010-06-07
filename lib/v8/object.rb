
module V8
  class Object
    include Enumerable

    def initialize(native, context = nil)
      @native = native
      @context = context || C::Context::GetEntered()
      raise ScriptError, "V8::Object.new called without an open V8 context" unless @context
    end

    def [](key)
      @context.enter do
        To.rb(@native.Get(To.v8(key)))
      end
    end

    def []=(key, value)
      value.tap do
        @context.enter do
          @native.Set(To.v8(key), To.v8(value))
        end
      end
    end

    def to_s
      @context.enter do
        To.rb(@native.ToString())
      end
    end

    def each
      @context.enter do
        for prop in To.rb(@native.GetPropertyNames())
          yield prop, self[prop]
        end
      end
    end

    def respond_to?(method)
      self[method] != nil
    end

    def method_missing(name, *args, &block)
      return super(name, *args, &block) unless self.respond_to?(name)
      property = self[name]
      if property.kind_of?(V8::Function)
        property.methodcall(self, *args)
      elsif args.empty?
        property
      else
        raise ArgumentError, "wrong number of arguments (#{args.length} for 0)" unless args.empty?
      end
    end
  end
end

class Object  
  def eval_js(javascript)
    V8::Context.new(:with => self).eval(javascript)
  end
end