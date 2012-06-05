require 'spec_helper'

describe V8::C::Script do
  it "can run a script and return a polymorphic result" do
    source = V8::C::String::New("(new Array())")
    filename = V8::C::String::New("<eval>")
    script = V8::C::Script::New(source, filename)
    result = script.Run()
    result.should be_kind_of V8::C::Array
  end
end