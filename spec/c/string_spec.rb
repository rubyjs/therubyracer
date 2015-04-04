require 'c_spec_helper'

describe V8::C::String do
  requires_v8_context

  it 'is a primitive' do
    expect(V8::C::String.NewFromUtf8(@isolate, 'test')).to be_a V8::C::Primitive
  end

  it 'can create new strings' do
    expect(V8::C::String.NewFromUtf8(@isolate, 'test')).to be
  end

  it 'can be converted to a ruby string' do
    expect(V8::C::String.NewFromUtf8(@isolate, 'test').Utf8Value).to eq 'test'
  end

  it 'can hold Unicode values outside the Basic Multilingual Plane' do
    string = V8::C::String.NewFromUtf8(@isolate, "\u{100000}")
    expect(string.Utf8Value).to eq "\u{100000}"
  end

  it 'can concatenate strings' do
    string_one = V8::C::String.NewFromUtf8(@isolate, 'Hello ')
    string_two = V8::C::String.NewFromUtf8(@isolate, 'World!')

    expect(V8::C::String.Concat(string_one, string_two).Utf8Value).to eq 'Hello World!'
  end
end
