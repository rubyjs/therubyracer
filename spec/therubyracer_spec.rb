require File.dirname(__FILE__) + '/spec_helper.rb'

describe "The Ruby Racer" do
  
  before(:each) do
    @cxt = V8::Context.new
  end
  
  describe  "Type Conversion from Ruby to Javascript" do
    
    it "can pass nil back to ruby" do
      eval("null").should be_nil      
    end
    
    it "passes back undefined value as nil" do
      eval("this.undefined").should be_nil
    end
    
    it "can pass strings back to ruby" do
      eval("'Hello World'").should == "Hello World"
    end
    
    it "can pass large strings back to ruby" do
      lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis faucibus, diam vel pellentesque aliquet, nisl sapien molestie eros, vitae vehicula libero massa vel neque. Phasellus tempor pharetra ipsum vel venenatis. Quisque vitae nisl vitae quam mattis pellentesque et in sapien. Sed at lectus quis eros pharetra feugiat non ac neque. Vivamus lacus eros, feugiat at volutpat at, viverra id nisl. Vivamus ac dolor eleifend libero venenatis pharetra ut iaculis arcu. Donec neque nibh, vehicula non porta a, consectetur eu erat. Sed eleifend, metus vel euismod placerat, lectus lectus sollicitudin nisl, ac elementum sem quam nec dolor. In hac habitasse platea dictumst. Proin vitae suscipit orci. Suspendisse a ipsum vel lorem tempus scelerisque et vitae neque. Proin sodales, tellus sit amet consequat cursus, odio massa ultricies enim, eu fermentum velit lectus in lacus. Quisque eu porttitor diam. Nunc felis purus, facilisis non tristique ac, pulvinar nec nulla. Duis dolor risus, egestas nec tristique ac, ullamcorper cras amet."
      eval("'#{lorem}'").should == lorem
    end
    
    it "can pass the empty string back to ruby" do
      eval("''").should == ""
    end
    
    it "can pass doubles back to ruby" do
      eval("2.5").should == 2.5
    end
    
    it "can pass fixed numbers back to ruby" do
      eval("1").should == 1
    end
    
    it "can pass boolean values back to ruby" do
      eval("true").should be(true)
      eval("false").should be(false)
    end
    
    it "can pass objects back to ruby" do
      eval("({foo: 'bar', baz: 'bang', '5': 5, segfault: {}})").tap do |object|
        object.should_not be_nil        
        object['foo'].should == 'bar'
        object['baz'].should == 'bang'
        object['5'].should == 5
        object['segfault'].should be_kind_of(V8::JSObject)
      end
    end  
                
  end
    
  describe "Calling Ruby Code from JavaScript" do
    
    it "knows your name" do
      class Foo
        def call
        end
      end
      
      foo = Foo.new
      #V8.what_is_this? Class.new
      # V8.what_is_this? Module.new
      # V8.what_is_this? Object.new
      # V8.what_is_this? :foo
      # V8.what_is_this? V8::JSObject.new
      # V8.what_is_this?(proc {|foo| "string form is: #{foo}"})
      # V8.what_is_this?(foo.method(:bar))
      V8.what_is_this?(foo)
    end
    
    it "can embed a ruby closure and call it from javascript" do
      # pending
      eval('times(5,2)', :times => lambda {|lhs, rhs| lhs * rhs}).should == 10
    end
    
    it "can call a bound ruby method" do
      pending      
      five = singleton(5) do
        def initialize(lhs)
          @lhs = lhs
        end
        def times(rhs)
          @lhs * rhs
        end
      end      
      eval('timesfive(3)', :timesfive => mult.method(:times)).should == 15
    end
    
    it "can embed a ruby object and call its methods" do
      pending
      eval('object.times(8,8)', :object => singelton {
        def times(lhs, rhs)
          lhs * rhs
        end
      })
    end
    
    it "unwraps ruby objects returned by embedded ruby code to maintain referential integrity" do
      pending
      mock(:object).tap do |o|
        eval('get()', :get => lambda {o}).should be(o)
      end
    end
  end
  
  # describe "Passing Ruby Values Back from Javascript"  
    
  def eval(str, scope = nil)
    @cxt.eval(str)
  end
  
  def singleton(*args, &body)
    Class.new.class_eval(&body).new(*args)
  end
  
end
