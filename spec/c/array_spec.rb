require 'spec_helper'

describe V8::C::Array do
  include C::ContextHelper
  it "can store and retrieve a value" do
    V8::C::HandleScope() do
      o = V8::C::Object::New()
      a = V8::C::Array::New()
      a.Length().should eql 0
      a.Set(0, o)
      a.Length().should eql 1
      a.Get(0).Equals(o).should be_true
    end
  end

  it "can be initialized with a length" do
    V8::C::HandleScope() do
      a = V8::C::Array::New(5)
      a.Length().should eql 5
    end
  end
end