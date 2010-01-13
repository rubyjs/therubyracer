
require "#{File.dirname(__FILE__)}/../spec_helper.rb"

include V8

describe C::Context do  
  
  it "should not have a current context if no context is open" do
    C::Context::GetCurrent().should be_nil
  end
  
  it "can determine if there is a current context" do
    C::Context::InContext().should be(false)
    C::Context.new.open do |cxt|
      C::Context::InContext().should be(true)
    end
  end
  
  it "returns the currently open context" do
    C::Context.new.open do |cxt|
      cxt.should be_eql(C::Context::GetCurrent())
      cxt.should == C::Context::GetCurrent()
    end
  end
end