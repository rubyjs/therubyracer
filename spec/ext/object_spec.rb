require 'spec_helper'

describe V8::C::Object do
  include V8::ExtSpec

  it "always returns a copy of the same object if it is the same object" do
    v8_eval('var o = new Object()')
    v8_eval('o').should be(v8_eval('o'))
  end

  it "will return a new peer and not barf if the old peer has been garbage collected" do
    v8_eval('var o = {foo: "bar"}')
    o = v8_eval('o')
    old_id = o.object_id
    ruby_gc do
      o = nil
      v8_eval('o').Get(c::String::New("foo")).Utf8Value().should == "bar"
      v8_eval('o').object_id.should_not be(old_id)
    end
  end
end