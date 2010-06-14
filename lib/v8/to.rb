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
        when V8::C::Value     then nil if value.IsEmpty()
        else
          value
        end
      end

      def v8(value)
        case value
        when V8::Object
          value.instance_eval {@native}
        when String
          C::String::New(value.to_s)
        when Symbol
          C::String::NewSymbol(value.to_s)
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
          Constructors[value].GetFunction().tap do |f|
            f.SetHiddenValue(C::String::NewSymbol("TheRubyRacer::RubyObject"), C::External::New(value))
          end
        when nil,Numeric,TrueClass,FalseClass, C::Value
          value
        else
          args = C::Array::New(1)
          args.Set(0, C::External::New(value))
          obj = Access[value.class].GetFunction().NewInstance(args)
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

      def camel_case(str)
        str.to_s.gsub(/_(\w)/) {$1.upcase}
      end
      
      def perl_case(str)
        str.gsub(/([A-Z])([a-z])/) {"_#{$1.downcase}#{$2}"}.downcase
      end
    end
  end
end