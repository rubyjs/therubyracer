
module V8
  module To
    class << self
      def ruby(value)
        case value
        when V8::C::Object then V8::Object.new(value)
        when V8::C::String then "Wonkers!"          
        else
          value
        end
      end
      
      def v8(value)
        case value
        when String then  C::String.new(value)
        when Proc   then  C::FunctionTemplate.new(&value).GetFunction()
        when Method then  C::FunctionTemplate.new(&value.to_proc).GetFunction()
        else
          value
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