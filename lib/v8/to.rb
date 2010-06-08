require 'weakref'

module V8
  module To
    class << self
      def rb(value)
        case value
        when V8::C::Function  then peer(value) {V8::Function}
        when V8::C::Array     then peer(value) {V8::Array}
        when V8::C::Object    then peer(value) {V8::Object}
        when V8::C::String    then value.Utf8Value()
        when V8::C::Date      then Time.at(value.NumberValue())
        else
          value
        end
      end

      def v8(value)
        case value
        when V8::Object
          value.instance_eval {@native}
        when String, Symbol
          C::String::New(value.to_s)
        when Proc,Method
          template = C::FunctionTemplate::New() do |arguments|
            rbargs = []
            for i in 0..arguments.Length() - 1
              rbargs << To.rb(arguments[i])
            end
            V8::Function.rubycall(value, *rbargs)
          end
          return template.GetFunction()
        when ::Array
          C::Array::New(value.length).tap do |a|
            value.each_with_index do |item, i|
              a.Set(i, To.v8(item))
            end
          end
        when ::Hash
          C::Object::New().tap do |o|
            value.each do |key, value|
              o.Set(To.v8(key), To.v8(value))
            end
          end
        when ::Time
          C::Date::New(value)
        when ::Class
          To.class_template(value).GetFunction().tap do |f|
            f.SetHiddenValue(C::String::NewSymbol("TheRubyRacer::RubyObject"), C::External::New(value))
          end
        when nil,Numeric,TrueClass,FalseClass, C::Value
          value
        else
          args = C::Array::New(1)
          args.Set(0, C::External::New(value))
          obj = To.class_template(value.class).GetFunction().NewInstance(args)
          return obj
        end
      end

      def peer(value)
        external = value.GetHiddenValue(C::String::NewSymbol("TheRubyRacer::RubyObject"))
        if external && !external.IsEmpty()
          external.Value()
        else
          yield.new(value)
        end
      end

      def template
        @rubyobject ||= C::ObjectTemplate::New().tap do |t|
          t.SetNamedPropertyHandler(
            NamedPropertyGetter,
            NamedPropertySetter,
            nil,
            nil,
          NamedPropertyEnumerator
          )
        end
      end

      def class_template(cls)
        @classes ||= {}
        if ref = @classes[cls.object_id]
          if ref.weakref_alive?
            ref.__getobj__
          else
            @classes.delete(cls.object_id)
            self.class_template(cls)
          end
        else
          class_template = C::FunctionTemplate::New() do |arguments|
            if arguments.Length() > 0 && arguments[0].kind_of?(C::External)
              wrapper = arguments[0]
            else
              rbargs = []
              for i in 0..arguments.Length() - 1
                rbargs << To.rb(arguments[i])
              end
              instance = V8::Function.rubycall(cls.method(:new), *rbargs)
              wrapper = C::External::New(instance)
            end
            arguments.This().tap do |this|
              this.SetHiddenValue(C::String::NewSymbol("TheRubyRacer::RubyObject"), wrapper)              
            end
          end
          class_template.InstanceTemplate().SetNamedPropertyHandler(
            NamedPropertyGetter,
            NamedPropertySetter,
            nil,
            nil,
            NamedPropertyEnumerator
          )
          if cls.name && cls.name =~ /(::)?(\w+?)$/
            class_template.SetClassName(C::String::NewSymbol($2))
          else
            class_template.SetClassName("Ruby")
          end
          @classes[cls.object_id] = WeakRef.new(class_template)
          class_template
        end
      end

      def camel_case(str)
        str.to_s.gsub(/_(\w)/) {$1.upcase}
      end
      
      def perl_case(str)
        str.gsub(/([A-Z])([a-z])/) {"_#{$1.downcase}#{$2}"}.downcase
      end
    end
  end
end