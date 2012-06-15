class V8::Conversion
  module Object
    include V8::Util::Weakcell

    def to_v8
      object = to_v8_template.NewInstance()
      V8::Context.link self, object
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

    class Get
      extend V8::Error::Protect
      def self.call(property, info)
        protect do
          context = V8::Context.current
          access = context.access
          object = info.Data().Value()
          name = property.Utf8Value()
          dontintercept = proc do
            return V8::C::Value::Empty
          end
          context.to_v8 access.get(object, name, &dontintercept)
        end
      end
    end

    class Set
      extend V8::Error::Protect
      def self.call(property, value, info)
        protect do
          context = V8::Context.current
          access = context.access
          object = info.Data().Value()
          name = property.Utf8Value()
          dontintercept = proc do
            return V8::C::Value::Empty
          end
          access.set(object, name, context.to_ruby(value), &dontintercept)
        end
      end
    end

    class IGet
      extend V8::Error::Protect
      def self.call(index, info)
        protect do
          context = V8::Context.current
          access = context.access
          object = info.Data().Value()
          dontintercept = proc do
            return V8::C::Value::Empty
          end
          context.to_v8 access.iget(object, index, &dontintercept)
        end
      end
    end

    class ISet
      extend V8::Error::Protect
      def self.call(index, value, info)
        protect do
          context = V8::Context.current
          access = context.access
          object = info.Data().Value()
          dontintercept = proc do
            return V8::C::Value::Empty
          end
          access.iset(object, index, context.to_ruby(value), &dontintercept)
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