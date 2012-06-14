class V8::Conversion
  module Proc
    include V8::Util::Weakcell

    def to_v8
      fn = to_v8_template.GetFunction()
      V8::Context.link self, fn
      return fn
    end

    def to_v8_template
      weakcell(:v8_template) do
        template = V8::C::FunctionTemplate::New()
        template.SetCallHandler(InvocationHandler.new, V8::C::External::New(self))
        template
      end
    end

    class InvocationHandler
      def call(arguments)
        context = V8::Context.current
        proc = arguments.Data().Value()
        length_of_given_args = arguments.Length()
        args = ::Array.new(proc.arity < 0 ? length_of_given_args : proc.arity)
        0.upto(args.length - 1) do |i|
          if i < length_of_given_args
            args[i] = context.to_ruby arguments[i]
          end
        end
        context.to_v8 proc.call(*args)
      rescue Exception => e
        warn "unhandled exception in ruby #{e.class}: #{e.message}"
        nil
      end
    end
  end
end