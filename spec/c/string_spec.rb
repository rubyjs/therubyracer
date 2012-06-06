require 'spec_helper'

describe V8::C::String do
  it "can hold Unicode values outside the Basic Multilingual Plane" do
    string = V8::C::String::New("\u{100000}")
    string.Utf8Value().should eql "\u{100000}"
  end
end