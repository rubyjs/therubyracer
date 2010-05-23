
module V8
  module To
    class << self
      def ruby(value)
        case value
        when V8::C::Function
          V8::Function.new(value).tap do |f|
            f.instance_eval do
              @native.instance_variable_set(:@context, C::Context::GetEntered())
            end
          end
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

      def camel_case(str)
        str.to_s.gsub(/_(\w)/) {$1.upcase}
      end
      
      def perl_case(str)
        str.gsub(/([A-Z])([a-z])/) {"_#{$1.downcase}#{$2}"}.downcase
      end
    end
  end
end