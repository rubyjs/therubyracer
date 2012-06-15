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
        prototype = template.InstanceTemplate()
        prototype.SetNamedPropertyHandler(Get, Set)
        prototype.SetIndexedPropertyHandler(IGet, ISet)
        template
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

    module Accessor
      include V8::Error::Protect
      def intercept(info, key, &block)
        context = V8::Context.current
        access = context.access
        object = info.Data().Value()
        handles_property = true
        dontintercept = proc do
          handles_property = false
        end
        protect do
          result = block.call(context, access, object, context.to_ruby(key), dontintercept)
          handles_property ? context.to_v8(result) : V8::C::Value::Empty
        end
      end
    end

    class Get
      extend Accessor
      def self.call(property, info)
        intercept(info, property) do |context, access, object, key, dontintercept|
          access.get(object, key, &dontintercept)
        end
      end
    end

    class Set
      extend Accessor
      def self.call(property, value, info)
        intercept(info, property) do |context, access, object, key, dontintercept|
          access.set(object, key, context.to_ruby(value), &dontintercept)
        end
      end
    end

    class IGet
      extend Accessor
      def self.call(property, info)
        intercept(info, property) do |context, access, object, key, dontintercept|
          access.iget(object, key, &dontintercept)
        end
      end
    end

    class ISet
      extend Accessor
      def self.call(property, value, info)
        intercept(info, property) do |context, access, object, key, dontintercept|
          access.iset(object, key, context.to_ruby(value), &dontintercept)
        end
      end
    end

  end
end