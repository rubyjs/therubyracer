require "#{File.dirname(__FILE__)}/../spec_helper.rb"

include V8

describe C::Function do
  it "is callable" do
    Context.new.open do |cxt|
      f = cxt.eval('(function() {return "Hello World"})', '<eval>');      
      f.call(nil).should == "Hello World"      
    end
  end
  
  it "receives proper argument length from ruby" do
    Context.new.open do |cxt|
      f = cxt.eval('(function() {return arguments.length})', 'eval')
      f.call(nil,1, 2, 3).should == 3
    end
  end
  
  it "maps all arguments from ruby" do
    Context.new.open do |cxt|
      f = cxt.eval('(function(one, two, three) {return one + two + three})', 'eval')
      f.call(nil, 1,2,3).should == 6
    end
  end
  
  it "properly maps ruby objects back and forth from arguments to return value" do
    Context.new.open do |cxt|
      Object.new.tap do |this|
         f = cxt.eval('(function() {return this})', 'eval')
         f.call(this).should be(this)
      end
    end
  end 
  
  it "can be called outside of a context" do
    Context.new.open do |cxt|
      @f = cxt.eval('(function() {return "Call Me"})', 'eval')
    end
    @f.call(nil).should == "Call Me"
  end
end