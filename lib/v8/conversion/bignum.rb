class V8::Conversion
  module Bignum
    def to_ruby
      self
    end

    def to_v8
      self.to_f.to_v8
    end
  end
end
