class V8::Conversion
  module Proc
    include V8::Util::Weakcell

    def to_v8
      fn = to_v8_template.GetFunction()
      V8::Context.link self, fn
      return fn
    end

    def to_v8_template
      weakcell(:v8_template) {V8::C::FunctionTemplate::New(InvocationHandler.new(self))}
    end

    class InvocationHandler
      include V8::Error::Protect

      def initialize(proc)
        @proc = proc
      end

      def call(arguments)
        protect do
          context = V8::Context.current
          length_of_given_args = arguments.Length()
          args = ::Array.new(@proc.arity < 0 ? length_of_given_args : @proc.arity)
          0.upto(args.length - 1) do |i|
            if i < length_of_given_args
              args[i] = context.to_ruby arguments[i]
            end
          end
          context.to_v8 @proc.call(*args)
        end
      end
    end
  end
end