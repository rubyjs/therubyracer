require "#{File.dirname(__FILE__)}/../spec_helper.rb"

include V8

describe C::Object do
  it "has a reference to its calling context" do
    C::Context.new.open do |cxt|
      o = cxt.eval('new Object()');
      o.context.should == cxt
      o.context.should be_eql(cxt)
    end
  end
end