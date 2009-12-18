
module V8
  module To
    class << self
      def ruby(value)
        case value
        when V8::C::Object then V8::Object.new(value)
        else
          value
        end
      end
      
      def v8(value)
        case value
        when String then  C::String.new(value)
        when Proc   then  C::FunctionTemplate.new(&value).GetFunction()
        else
          value
        end
      end
    end
  end
end