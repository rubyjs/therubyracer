require 'c_spec_helper'

describe V8::C::Isolate do
  it 'can create a new isolate' do
    expect(V8::C::Isolate.New).to be
  end
end
