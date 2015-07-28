require 'c_spec_helper'

describe V8::C::Isolate do
  let(:isolate) { V8::C::Isolate::New() }

  it 'can create a new isolate' do
    expect(isolate).to be
  end
end
