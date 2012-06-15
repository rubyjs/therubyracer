require 'spec_helper'

describe V8::Error do
  it "uses the same ruby exception through multiple language boundaries" do
    V8::Context.new do |cxt|
      error = StandardError.new('potato')
      cxt['one'] = lambda do
        cxt.eval('two()', 'one.js')
      end
      cxt['two'] = lambda do
        cxt.eval('three()', 'two.js')
      end
      cxt['three'] = lambda do
        raise error
      end
      lambda {
        cxt.eval('three()')
      }.should raise_error {|e| e.should be error}
    end
  end
end
