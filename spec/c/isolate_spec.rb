require 'c_spec_helper'

describe V8::C::Isolate do
  let(:isolate) { V8::C::Isolate::New() }

  it 'can create a new isolate' do
    expect(isolate).to be
  end

  it 'can be tested for equality' do
    expect(isolate.Equals(isolate)).to eq true
    expect(isolate.Equals(V8::C::Isolate::New())).to eq false
  end

  it "can be disposed of" do
    isolate.Dispose()
  end

  it "has has heap spaces" do
    expect(isolate.NumberOfHeapSpaces()).to be >= 1
  end
end
