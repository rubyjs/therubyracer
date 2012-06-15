class V8::Conversion
  module Object
    include V8::Util::Weakcell

    def to_v8
      context = V8::Context.current
      constructor = context.to_v8(self.class)
      object = constructor.NewInstance([V8::C::External::New(self)])
      return object
    end

    def to_v8_template
      weakcell(:v8_template) do
        V8::C::ObjectTemplate::New().tap do |template|
          data = V8::C::External::New(self)
          template.SetNamedPropertyHandler(Get, Set, nil, nil, nil, data)
          template.SetIndexedPropertyHandler(IGet, ISet, nil, nil, nil, data)
        end
      end
    end

    def to_ruby
      self
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

  module NativeObject
    def to_ruby
      wrap = if IsArray()
        ::V8::Array
      elsif IsFunction()
        ::V8::Function
      else
        ::V8::Object
      end
      wrap.new(self)
    end

    def to_v8
      self
    end
  end
end