require 'v8/init'

describe V8::C::V8 do
  it 'can say its version' do
    expect(V8::C::V8.GetVersion).to be_a String
  end
end
