
require "#{File.dirname(__FILE__)}/../spec_helper.rb"

include V8

describe C::Context do  
  
  it "should not have a current context if no context is open" do
    C::Context::GetEntered().should be_nil
  end

end