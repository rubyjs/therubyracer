require 'c_spec_helper'

describe V8::C::Array do
  requires_v8_context

  it 'can store and retrieve a value' do
    o = V8::C::Object::New(@isolate)
    a = V8::C::Array::New(@isolate)

    expect(a.Length).to eq 0

    a.Set(0, o)
    expect(a.Length).to eq 1

    expect(a.Get(0).Equals(o)).to eq true
  end

  it 'can be initialized with a length' do
    a = V8::C::Array::New(@isolate, 5)

    expect(a.Length).to eq 5
  end
end
