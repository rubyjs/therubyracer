module V8
  class Context
    attr_reader :native
    def initialize
      @native = V8::C::Context::New()
    end

    def enter(&block)
      if !entered?
        lock_scope_and_enter(&block)
      else
        yield
      end
    end

    def entered?
      Context.current == self
    end

    def self.current
      Thread.current[:v8_context]
    end

    def self.current=(context)
      Thread.current[:v8_context] = context
    end

    def lock_scope_and_enter
      current = Context.current
      Context.current = self
      V8::C::Locker() do
        V8::C::HandleScope() do
          begin
            @native.Enter()
            yield if block_given?
          ensure
            @native.Exit()
          end
        end
      end
    ensure
      Context.current = current
    end

    def eval(source, filename = '<eval>', line = 1)
      enter do
        source = V8::C::String::New(source.to_s)
        filename = V8::C::String::New(filename.to_s)
        script = V8::C::Script::New(source, filename)
        result = script.Run()
        result.kind_of?(V8::C::String) ? result.Utf8Value() : result
      end
    end

    def []=(key, value)
      enter do
        @native.Global().Set(to_v8(key), to_v8(value))
      end
      return value
    end

    def [](key)
      enter do
        to_ruby(@native.Global().Get(to_v8(key)))
      end
    end

    def to_ruby(v8_object)
      case v8_object
      when V8::C::String then v8_object.Utf8Value()
      when V8::C::Value then v8_object.ToString().Utf8Value()
      else
        v8_object
      end
    end

    def to_v8(ruby_object)
      # ruby_object.respond_to?(:to_v8) ? ruby_object.to_v8 : V8::C::Object::New()
      case ruby_object
      when Numeric then ruby_object
      when String then V8::C::String::New(ruby_object)
      else
        V8::C::Object::New()
      end
    end
  end
end