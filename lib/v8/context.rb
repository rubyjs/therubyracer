require 'stringio'
module V8
  class Context
    include V8::Error::Try
    attr_reader :native, :conversion, :access

    def initialize(options = {})
      @conversion = Conversion.new
      @access = Access.new
      if global = options[:with]
        Context.new.enter do
          global_template = global.class.to_template.InstanceTemplate()
          @native = V8::C::Context::New(nil, global_template)
        end
        enter {link global, @native.Global()}
      else
        @native = V8::C::Context::New()
      end
      yield self if block_given?
    end

    def scope
      enter { to_ruby @native.Global() }
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

    def load(filename)
      File.open(filename) do |file|
        self.eval file, filename
      end
    end

    def eval(source, filename = '<eval>', line = 1)
      if IO === source || StringIO === source
        source = source.read
      end
      enter do
        source = V8::C::String::New(source.to_s)
        filename = V8::C::String::New(filename.to_s)
        script = try { V8::C::Script::New(source, filename) }
        to_ruby try {script.Run()}
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