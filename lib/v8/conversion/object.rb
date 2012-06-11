class V8::Conversion
  module Object
    def to_v8
      template = V8::C::ObjectTemplate::New()
      template.SetNamedPropertyHandler(Get, nil, nil, nil, nil, V8::C::External::New(self))
      instance = template.NewInstance()
      V8::Context.link self, instance
      return instance
    end

    def to_ruby
      self
    end

    class Get
      def self.call(property, info)
        context = V8::Context.current
        access = context.access
        object = info.Data().Value()
        name = property.Utf8Value()
        dontintercept = proc do
          return V8::C::Empty
        end
        context.to_v8 access.get(object, name, &dontintercept)
      rescue Exception => e
        warn "uncaught exception: #{e.class}: #{e.message} while accessing object property: #{e.backtrace.join('\n')}"
      end
    end
  end

  module NativeObject
    def to_ruby
      ::V8::Object.new(self)
    end
  end
end