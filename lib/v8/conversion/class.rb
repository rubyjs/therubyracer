class V8::Conversion
  module Class
    include V8::Util::Weakcell

    def to_v8
      weakcell(:v8_constructor) do
        V8::C::FunctionTemplate::New(Constructor.new(self))
      end.GetFunction()
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