require 'bigdecimal'
class V8::Conversion
  module BigDecimal
    def to_v8
      context = V8::Context.current
      constructor = context.to_v8(self.class)
      object = constructor.NewInstance([V8::C::External::New(self)])
      return object
    end
  end
end