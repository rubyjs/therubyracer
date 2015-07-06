require 'c_spec_helper'

describe V8::C::Context do
  let(:isolate) { V8::C::Isolate::New() }
  let(:context) { V8::C::Context::New(isolate) }

  it "can be instantiated" do
    expect(context).to be
  end
end
