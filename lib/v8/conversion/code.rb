class V8::Conversion
  module Code
    include V8::Util::Weakcell

    def to_v8
      fn = to_template.GetFunction()
      V8::Context.link self, fn
      return fn
    end

    def to_template
      weakcell(:template) {V8::C::FunctionTemplate::New(InvocationHandler.new(self))}
    end

    class InvocationHandler
      include V8::Error::Protect

      def initialize(code)
        @code = code
      end

      def call(arguments)
        protect do
          context = V8::Context.current
          length_of_given_args = arguments.Length()
          args = ::Array.new(@code.arity < 0 ? length_of_given_args : @code.arity)
          0.upto(args.length - 1) do |i|
            if i < length_of_given_args
              args[i] = context.to_ruby arguments[i]
            end
          end
          context.to_v8 @code.call(*args)
        end
      end
    end
  end
end