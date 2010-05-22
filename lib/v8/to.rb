
module V8
  module To
    class << self
      def ruby(value)
        case value
        when V8::C::Function  then V8::Function.new(value)
        when V8::C::Array     then V8::Array.new(value)          
        when V8::C::Object    then V8::Object.new(value)
        when V8::C::String    then value.Utf8Value()
        else
          value
        end
      end

      alias_method :rb, :ruby

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
              rbargs << To.ruby(arguments[i])
            end
            To.v8(value.call(*rbargs))
          end
          return template.GetFunction()
        when nil,Numeric
          value
        else
          rubyobject = C::ObjectTemplate::New()
          rubyobject.SetNamedPropertyHandler(
            NamedPropertyGetter,
            NamedPropertySetter,
            nil,
            nil,
            NamedPropertyEnumerator
          )
          obj = nil
          unless C::Context::InContext()
            cxt = C::Context::New()
            cxt.Enter()
            begin
              obj = rubyobject.NewInstance()
              obj.SetHiddenValue(C::String::New("TheRubyRacer::RubyObject"), C::External::Wrap(value))
            ensure
              cxt.Exit()
            end
          else
            obj = rubyobject.NewInstance()
            obj.SetHiddenValue(C::String::New("TheRubyRacer::RubyObject"), C::External::Wrap(value))
          end
          return obj
        end
      end

      class NamedPropertyGetter
        def self.call(property, info)
          name = To.rb(property)
          obj = To.rb(info.This())
          perl_name = To.perl_case(name)
          methods = obj.public_methods(false).map(&:to_s)
          method_name = if methods.include?(name)
            name
          elsif methods.include?(perl_name)
            perl_name
          end
          if method_name
            method = obj.method(method_name)
            if method.arity == 0
              To.v8(method.call())
            else
              To.v8(method)
            end
          else
            C::Empty
          end
        end
      end

      class NamedPropertySetter
        def self.call(property, value, info)
          obj = To.rb(info.This())
          setter = To.rb(property) + "="
          camel_name = To.camel_case(setter)
          perl_name = To.perl_case(setter)
          methods = obj.public_methods(false).map(&:to_s)
          if methods.include?(perl_name)
            obj.send(perl_name, To.rb(value))
          elsif methods.include?(camel_name)
            obj.send(camel_name, To.rb(value))
          else
             C::Empty
          end
        end
      end
      
      class NamedPropertyEnumerator
        def self.call(info)
          obj = To.rb(info.This())
          camel_methods = obj.public_methods(false).inject(Set.new) do |set, name|
            set.tap do
              set << To.camel_case(name)
            end
          end
          names = V8::C::Array::New(camel_methods.length)
          camel_methods.each_with_index do |name, i|
            names.Set(i, C::String::New(name))
          end
          return names
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