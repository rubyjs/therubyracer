require 'c_spec_helper'

describe V8::C::String do
  requires_v8_context

  it 'can create new strings' do
    expect(V8::C::String.NewFromUtf8('test')).to be
  end

  it 'can be converted to a ruby string' do
    expect(V8::C::String.NewFromUtf8('test').Utf8Value).to eq 'test'
  end

  it 'can hold Unicode values outside the Basic Multilingual Plane' do
    string = V8::C::String.NewFromUtf8("\u{100000}")
    expect(string.Utf8Value).to eq "\u{100000}"
  end

  it 'can concatenate strings' do
    string_one = V8::C::String.NewFromUtf8('Hello ')
    string_two = V8::C::String.NewFromUtf8('World!')

    expect(V8::C::String.Concat(string_one, string_two).Utf8Value).to eq 'Hello World!'
  end

  it 'can naturally translate ruby strings into v8 strings' do
    expect(V8::C::String.Concat(V8::C::String.NewFromUtf8('Hello '), 'World').Utf8Value).to eq 'Hello World'
  end

  it 'can naturally translate ruby objects into v8 strings' do
    expect(V8::C::String.Concat(V8::C::String.NewFromUtf8('forty two is '), 42).Utf8Value).to eq 'forty two is 42'
  end
end
