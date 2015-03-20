require 'c_spec_helper'

describe V8::C::Object do
  requires_v8_context

  it 'can create a new object' do
    expect(V8::C::Object.New).to be
  end
end
