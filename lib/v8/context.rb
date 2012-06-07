module V8
  class Context
    include Conversion

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
        to_ruby script.Run()
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
  end
end