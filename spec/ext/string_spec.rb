
require 'spec_helper'

describe V8::C::String do
  include V8::ExtSpec
  
  describe "a string with null bytes" do
    subject {c::String::New("foo\0bar")}
    its(:Utf8Value) {should eql "foo\0bar"}
  end
end