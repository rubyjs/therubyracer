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
        object = info.Data().Value()
        name = property.Utf8Value()
        if object.respond_to?(name)
          if object.method(name).arity == 0
            context.to_v8 object.send(name)
          else
            context.to_v8 object.method(name)
          end
        else
          V8::C::Value::Empty
        end
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