require 'c_spec_helper'

describe V8::C::Value do
  requires_v8_context

  def to_value(value)
    object = V8::C::Object.New
    key = V8::C::String.NewFromUtf8('key')

    object.Set(key, value)
    object.Get(key)
  end

  it 'converts strings' do
    expect(to_value(V8::C::String.NewFromUtf8('value')).Utf8Value).to eq 'value'
  end

  it 'converts nil' do
    expect(to_value(nil)).to eq nil
  end

  it 'converts undefined to nil' do
    object = V8::C::Object.New
    key = V8::C::String.NewFromUtf8('key')

    expect(object.Get(key)).to eq nil
  end

  it 'converts FixNums' do
    expect(to_value(42)).to eq 42
  end

  it 'converts booleans' do
    expect(to_value(true)).to eq true
    expect(to_value(false)).to eq false
  end

  it 'converts objects' do
    object = V8::C::Object.New
    object.Set(V8::C::String.NewFromUtf8('test'), 1)

    expect(to_value(object)).to eq object
  end
end
