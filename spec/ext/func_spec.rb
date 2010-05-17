require "#{File.dirname(__FILE__)}/../spec_helper.rb"

include V8

describe C::Function do
  it "is callable" do
    C::Context.new.open do |cxt|
      f = cxt.eval('(function() {return "Hello World"})', '<eval>');      
      f.Call(cxt.Global).AsciiValue().should == "Hello World"      
    end
  end
  
  it "receives proper argument length from ruby" do
    C::Context.new.open do |cxt|
      f = cxt.eval('(function() {return arguments.length})', 'eval')
      f.Call(nil,1, 2, 3).should == 3
    end
  end
  
  it "maps all arguments from ruby" do
    C::Context.new.open do |cxt|
      f = cxt.eval('(function(one, two, three) {return one + two + three})', 'eval')
      f.Call(nil, 1,2,3).should == 6
    end
  end
  
  it "properly maps ruby objects back and forth from arguments to return value" do
    C::Context.new.open do |cxt|
      Object.new.tap do |this|
         f = cxt.eval('(function() {return this})', 'eval')
         f.Call(this).should be(this)
      end
    end
  end 
  
  it "can be called outside of a context" do
    C::Context.new.open do |cxt|
      @f = cxt.eval('(function() {return "Call Me"})', 'eval')
    end
    @f.Call(nil).Utf8Value().should == "Call Me"
  end
end