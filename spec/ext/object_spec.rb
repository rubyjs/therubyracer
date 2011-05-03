require 'spec_helper'

describe V8::C::Object do
  include V8::ExtSpec

  it "always returns a copy of the same object if it is the same object" do
    v8_eval('var o = new Object()')
    v8_eval('o').should be(v8_eval('o'))
  end
end