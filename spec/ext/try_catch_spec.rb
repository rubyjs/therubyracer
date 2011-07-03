require "#{File.dirname(__FILE__)}/../spec_helper.rb"

include V8

describe C::TryCatch do

  before {@lock = C::Locker.new;@cxt = C::Context::New();}
  after {@cxt.Dispose(); @lock.delete}

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

    it "doesn't segfault when an error is raised in a javascript function on a native prototype" do
      constructor = Class.new
      constructor.class_eval do
        def detonate(*a)
          raise "BOOM!"
        end
      end
      V8::Context.new do |cxt|
        cxt['Boom'] = constructor
        cxt['puts'] = method(:puts)
        danger = <<-JS
  Boom.prototype.boom = function() {
    this.detonate()
  }
  var go = new(Boom)()
  try {
    go.boom()
  } catch (e) {
  }
  go.boom()
  JS
        expect {cxt.eval(danger, 'danger.js')}.should raise_error(V8::JSError)
      end
    end

end