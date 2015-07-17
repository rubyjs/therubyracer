require 'c_spec_helper'

describe V8::C::Value do
  requires_v8_context

  def convert(value)
    V8::C::Value.FromRubyObject(@isolate, value).ToRubyObject
  end

  it 'converts strings' do
    expect(convert('value').Utf8Value).to eq 'value'
  end

  it 'converts nil' do
    expect(convert(nil)).to eq nil
  end

  it 'converts undefined to nil' do
    object = V8::C::Object.New(@isolate)
    key = V8::C::String.NewFromUtf8(@isolate, 'key')

    expect(object.Get(@ctx, key).FromJust()).to eq nil
  end

  it 'converts FixNums' do
    expect(convert(42).Value()).to eq 42
  end

  it 'converts booleans' do
    expect(convert(true)).to eq true
    expect(convert(false)).to eq false
  end

  it 'converts objects' do
    object = V8::C::Object.New(@isolate)
    object.Set(@ctx, V8::C::String.NewFromUtf8(@isolate, 'test'), 1)

    expect(convert(object)).to eq object
  end
end
