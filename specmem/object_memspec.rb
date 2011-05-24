
require 'spec_helper'

describe V8::C::Object do
  include V8::MemSpec
  it "will return a new peer and not barf if the old peer has been garbage collected" do
    v8_eval('var o = {foo: "bar"}')
    old_id = v8_eval('o').object_id
    ruby_gc do
      v8_eval('o').Get(c::String::New("foo")).Utf8Value().should == "bar"
      v8_eval('o').object_id.should_not be(old_id)
    end
  end
end