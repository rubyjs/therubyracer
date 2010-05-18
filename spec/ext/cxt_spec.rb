
require "#{File.dirname(__FILE__)}/../spec_helper.rb"

include V8

describe C::Context do  
  
  it "should not have a current context if no context is open" do
    C::Context::GetEntered().should be_nil
  end
  
  it "can determine if there is a current context" do
    C::Context::InContext().should be(false)
    Context.open do
      C::Context::InContext().should be(true)
    end
  end
    
end