module V8
  class Context
    attr_reader :native, :conversion, :access

    def initialize(options = {})
      @conversion = Conversion.new
      @access = Access.new
      if global = options[:with]
        V8::C::Locker() do
          V8::C::HandleScope() do
            tmp = V8::C::Context::New()
            tmp.Enter()
            global_template = global.to_v8_template
            tmp.Exit()
            @native = V8::C::Context::New(nil, global_template)
            enter { link global, @native.Global() }
          end
        end
      else
        @native = V8::C::Context::New()
      end
      yield self if block_given?
    end

    def to_ruby(v8_object)
      @conversion.to_ruby(v8_object)
    end

    def to_v8(ruby_object)
      @conversion.to_v8(ruby_object)
    end

    def link(ruby_object, v8_object)
      @conversion.equate ruby_object, v8_object
    end

    def self.link(*args)
      current.link *args
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
        V8::C::TryCatch() do |trycatch|
          source = V8::C::String::New(source.to_s)
          filename = V8::C::String::New(filename.to_s)
          script = V8::C::Script::New(source, filename)
          result = script.Run()
          if trycatch.HasCaught()
            exception = trycatch.Exception()
            if exception.IsNativeError()
              raise to_ruby exception.Get("message")
            else
              raise to_ruby exception.ToString()
            end
          else
            to_ruby result
          end
        end
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