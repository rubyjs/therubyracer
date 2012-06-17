require 'spec_helper'
require 'bigdecimal'
describe V8::Conversion do
  it "can embed BigDecimal values" do
    cxt = V8::Context.new
    cxt['big'] = BigDecimal.new('1.1')
    cxt['big'].should eql BigDecimal.new('1.1')
  end
end
