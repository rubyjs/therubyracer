class V8::Conversion
  module Fundamental
    def to_ruby(v8_object)
      v8_object.to_ruby
    end

    def to_v8(context, ruby_object)
      ruby_object.to_v8 context
    end
  end
end
