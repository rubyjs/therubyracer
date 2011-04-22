require "spec_helper"

describe V8::C::Array do
  include V8::ExtSpec
  
  it "can be instantiated" do
    a = c::Array::New()
    a.Length().should eql(0)
  end
  
  it "maintains referential integrity" do
    v8_eval('a = []')
    v8_eval('a').should be(v8_eval('a'))
  end
end
