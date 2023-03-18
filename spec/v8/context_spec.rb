require 'spec_helper'

describe V8::Context do
  describe '.dispose' do
    it "can be disposed of" do
      cxt = V8::Context.new
      cxt.enter do
        cxt['object'] = V8::Object.new
      end
      cxt.dispose()

      lambda {cxt.eval('1 + 1')}.should raise_error
      lambda {cxt['object']}.should raise_error
    end

    it "can be disposed of any number of times" do
      cxt = V8::Context.new
      10.times {cxt.dispose()}
    end
  end

  describe '.deep_clone' do
    let(:js) do
      <<-js
        if (typeof x === 'undefined') {
            x = 1;
        } else {
            x = x + 1;
        }
      js
    end

    it 'duplicates the underlying native object' do
      cxt = V8::Context.new

      cxt.eval(js)

      cloned_cxt = cxt.deep_clone

      cxt.eval(js)
      cxt["x"].should == 2

      cloned_cxt["x"].should == 1
      cloned_cxt.eval(js)
      cloned_cxt["x"].should == 2

      cxt.eval(js)
      cxt["x"].should == 3
    end

    it 'duplicates functions' do
      cxt = V8::Context.new

      cxt.eval('var val = {num: 5, isTruthy: function (arg) { return !!arg }}')

      cloned_cxt = cxt.deep_clone

      cloned_cxt["val"].isTruthy(1).should be(true)
    end

    it "keeps closures' contexts" do
      js = <<-js
        function wrapper(x) { return function() { return x; } };
        var f = wrapper(42);
      js

      cxt = V8::Context.new

      cxt.eval(js)

      cloned_cxt = cxt.deep_clone

      cloned_cxt.eval('x = f();')

      cloned_cxt["x"].should == 42
    end

    it 'works on empty contexts' do
      cxt = V8::Context.new

      cloned_cxt = cxt.deep_clone

      cxt.eval(js)
      cxt["x"].should == 1

      cloned_cxt["x"].should be(nil)
    end

    it 'keeps original options' do
      cxt = V8::Context.new(:timeout => 10)

      cloned_cxt = cxt.deep_clone

      cloned_cxt.timeout.should == 10
    end

    context 'when the original context has been disposed of' do
      it 'should not allow cloning any more' do
        cxt = V8::Context.new

        cxt.dispose

        lambda { cxt.deep_clone }.should raise_error
      end

      it 'should not affect previously cloned contexts' do
        cxt = V8::Context.new
        cxt.eval(js)

        cloned_cxt = cxt.deep_clone

        cxt.dispose

        cloned_cxt.eval(js)
        cloned_cxt["x"].should == 2
      end
    end
  end
end
