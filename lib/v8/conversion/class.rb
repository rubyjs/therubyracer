class V8::Conversion
  module Class
    include V8::Util::Weakcell

    def to_v8
      fn = to_v8_template.GetFunction()
      V8::Context.current.link self, fn
      return fn
    end

    def to_v8_template
      weakcell(:v8_constructor) do
        template = V8::C::FunctionTemplate::New(Constructor.new(self))
      end
    end

    class Constructor
      include V8::Error::Protect

      def initialize(cls)
        @class = cls
      end

      def call(arguments)
        protect do
          context = V8::Context.current
          args = ::Array.new(arguments.Length())
          0.upto(args.length - 1) do |i|
            args[i] = context.to_ruby arguments[i]
          end
          context.to_v8 @class.new(*args)
        end
      end
    end
  end
end