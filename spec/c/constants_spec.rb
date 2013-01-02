require 'spec_helper'

describe V8::C do
  it "has constant methods for Undefined, Null, True and False" do
    [:Undefined, :Null, :True, :False].each do |name|
      constant = V8::C.send(name)
      constant.should_not be_nil
      V8::C.send(name).should be constant
    end
  end

  it "has a value for the Empty handle" do
    V8::C::Value::Empty.should_not be_nil
    V8::C::Value::Empty.should be V8::C::Value::Empty
  end

  it "can access the V8 version" do
    V8::C::V8::GetVersion().should match /^3\.11/
  end

  it "can ignore OOM exceptions" do
    V8::C::V8::IgnoreOutOfMemoryException()
    V8::Context.new.eval('var a="0";while(true){a=a+a;}').should be_nil # This context should stop being evaluated at some point, and return nil
    V8::Context.new.eval('3*4').should be 12 # Other contexts should work just fine
  end
end
