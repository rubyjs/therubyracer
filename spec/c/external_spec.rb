require 'spec_helper'

describe V8::C::External do
  before do
    @cxt = V8::C::Context::New()
    @cxt.Enter()
  end
  after do
    @cxt.Exit()
  end
  it "can store and retrieve a value" do
    V8::C::HandleScope() do
      o = Object.new
      external = V8::C::External::New(o)
      external.Value().should be(o)
    end
  end
end