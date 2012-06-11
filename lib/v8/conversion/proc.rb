class V8::Conversion
  module Proc

    def to_v8
      return v8_template.GetFunction()
    end

    def v8_template
      unless @v8_template
        @v8_template = V8::C::FunctionTemplate::New()
        @v8_template.SetCallHandler(InvocationHandler.new, V8::C::External::New(self))
      end
      return @v8_template
    end

    class InvocationHandler
      def call(arguments)
        context = V8::Context.current
        proc = arguments.Data().Value()
        args = ::Array.new(arguments.Length())
        0.upto(args.length - 1) do |i|
          args[i] = context.to_ruby arguments[i]
        end
        context.to_v8 proc.call(*args)
      rescue Exception => e
        warn "unhandled exception in ruby #{e.class}: #{e.message}"
        nil
      end
    end
  end
end