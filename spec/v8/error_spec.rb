require File.expand_path(File.dirname(__FILE__) + '/../spec_helper')

describe V8::JSError do

  before(:each) do
    @cxt = V8::Context.new
    @cxt['one'] = lambda do
      @cxt.eval('two()', 'one.js')
    end
    @cxt['two'] = lambda do
      @cxt.eval('three()', 'two.js')
    end
  end

  it "captures a message without over nesting when the error is an error" do
    throw! do |e|
      e.message.should == "BOOM!"
    end
  end

  it "captures the js message without over nesting when the error is a normal object" do
    throw!('{foo: "bar"}') do |e|
      e.message.should == "[object Object]"
    end
    throw!('{message: "bar"}') do |e|
      e.message.should == "bar"
    end
  end

  it "captures a thrown value as the message" do
    throw!('"BOOM!"') do |e|
      e.message.should == "BOOM!"
    end
    throw!('6') do |e|
      e.message.should == '6'
    end
  end

  it "has a reference to the root javascript cause" do
    throw!('"I am a String"') do |e|
      e.should_not be_in_ruby
      e.should be_in_javascript 
      e.value.should == "I am a String"
    end
  end

  it "has a reference to the root ruby cause if one exists" do
    StandardError.new("BOOM!").tap do |bomb|
      @cxt['boom'] = lambda do
        raise bomb
      end
      lambda {
        @cxt.eval('boom()', 'boom.js')
      }.should(raise_error do |raised|
        raised.should be_in_ruby 
        raised.should_not be_in_javascript 
        raised.value.should be(bomb)
      end)
    end
  end

  describe "backtrace" do

    it "is mixed with ruby and javascript" do
      throw! do |e|
        e.backtrace.first.should == "at three.js:1:7"
        e.backtrace[1].should =~ /error_spec.rb/
        e.backtrace[2].should == "at two.js:1:1"
        e.backtrace[3].should =~ /error_spec.rb/
        e.backtrace[4].should == "at one.js:1:1"
      end
    end
    
    it "can be set to show only ruby frames" do
      throw! do |e|
        e.backtrace(:ruby).each do |frame|
          frame.should =~ /(\.rb|):\d+/
        end
      end
    end
    
    it "can be set to show only javascript frames" do
      throw! do |e|
        e.backtrace(:javascript).each do |frame|
          frame.should =~ /\.js:\d:\d/
        end
      end
    end
    
    it "includes a mystery marker when the original frame is unavailable because what got thrown wasn't an error" do
      throw!("6") do |e|
        e.backtrace.first.should == 'at three.js:1:1'
      end
    end
    
    it "has a source name and line number when there is a javascript SyntaxError" do
      lambda do
        @cxt.eval(<<-INVALID, 'source.js')
"this line is okay";
"this line has a syntax error because it ends with a colon":
"this line is also okay";
"how do I find out that line 2 has the syntax error?";
INVALID
      end.should raise_error(V8::JSError) {|error|
        error.backtrace.first.should == 'at source.js:2:60'
      }
    end

    it "can start with ruby at the bottom" do
      @cxt['boom'] = lambda do
        raise StandardError, "Bif!"
      end
      lambda {
        @cxt.eval('boom()', "boom.js")
      }.should(raise_error {|e|
        e.backtrace.first.should =~ /error_spec\.rb/
        e.backtrace[1].should =~ /boom.js/
      })
    end
  end
  
  
  def throw!(js = "new Error('BOOM!')", &block)
    @cxt['three'] = lambda do
      @cxt.eval("throw #{js}", 'three.js')
    end
    lambda do
      @cxt['one'].call()
    end.should(raise_error(V8::JSError, &block))
  end
end