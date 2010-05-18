require "#{File.dirname(__FILE__)}/../spec_helper.rb"

include V8

describe C::TryCatch do

  it "does not allow instance creation by default" do
    lambda {
     C::TryCatch.new 
    }.should raise_error
  end
  
  it "will do nothing if not passed a block" do
    C::TryCatch.try.should == nil
  end
  
  it "executes a block in the context of a C++ stack frame" do
    C::TryCatch.try do |catch|
      catch.HasCaught().should be(false)      
    end
  end
  
  it "raises an erro if you try to access it outside of its scope" do
    tc = C::TryCatch.try do |catch|
      catch.tap {}
    end
    lambda {
      tc.HasCaught()
    }.should raise_error(ScriptError)
  end
  
end