require 'c_spec_helper'

describe V8::C::Isolate do
  it 'can create a new isolate' do
    expect(V8::C::Isolate.New).to be
  end

  it 'can be tested for equality' do
    isolate_one = V8::C::Isolate.New
    isolate_two = V8::C::Isolate.New

    expect(isolate_one.Equals(isolate_one)).to eq true
    expect(isolate_one.Equals(isolate_two)).to eq false
  end
end
