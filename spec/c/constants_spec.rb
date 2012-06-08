require 'spec_helper'

describe V8::C do
  it "has constant methods for Undefined, Null, True and False" do
    [:Undefined, :Null, :True, :False].each do |name|
      constant = V8::C.send(name)
      constant.should_not be_nil
      V8::C.send(name).should be constant
    end
  end
end