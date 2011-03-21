require "#{File.dirname(__FILE__)}/../spec_helper.rb"

include V8

describe C::Function do
  it "is callable" do
    Context.new do |cxt|
      f = cxt.eval('(function() {return "Hello World"})', '<eval>');      
      f.call().should == "Hello World"      
    end
  end
  
  it "receives proper argument length from ruby" do
    Context.new do |cxt|
      f = cxt.eval('(function() {return arguments.length})', 'eval')
      f.call(1, 2, 3).should == 3
    end
  end
  
  it "maps all arguments from ruby" do
    Context.new do |cxt|
      f = cxt.eval('(function(one, two, three) {return one + two + three})', 'eval')
      f.call(1,2,3).should == 6
    end
  end
  
  it "properly maps ruby objects back and forth from arguments to return value" do
    Context.new do |cxt|
      Object.new.tap do |this|
         f = cxt.eval('(function() {return this})', 'eval')
         f.methodcall(this).should be(this)
      end
    end
  end 
  
  it "can be called outside of a context" do
    Context.new do |cxt|
      @f = cxt.eval('(function() {return "Call Me"})', 'eval')
    end
    @f.call().should == "Call Me"
  end
  
  it "is reflected properly" do
    Context.new do |cxt|
      cxt['say'] = lambda {|word, times| word * times}
      cxt.eval('say("Hello", 3)').should == "HelloHelloHello"
    end
  end
  
  it "has a name" do
    Context.new do |cxt|
      f = cxt.eval('(function hi() {return "Hello World"})', '<eval>')
      f.name.should == "hi"
    end
  end

  it "can have its name set" do
    Context.new do |cxt|
      f = cxt.eval('(function () {return "Goodbye World"})', '<eval>')
      f.name = 'bye'
      f.name.should == 'bye'
    end
  end
end