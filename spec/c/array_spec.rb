require 'c_spec_helper'

describe V8::C::Array do
  requires_v8_context

  it 'can store and retrieve a value' do
    o = V8::C::Object::New(@isolate)
    a = V8::C::Array::New(@isolate)

    expect(a.Length).to eq 0

    a.Set(@ctx, 0, o)
    expect(a.Length).to eq 1

    expect(a.Get(@ctx, 0)).to v8_eq o
  end

  it 'can be initialized with a length' do
    a = V8::C::Array::New(@isolate, 5)
    expect(a.Length).to eq 5
  end

  it 'can clone its elements' do
    o = V8::C::Object::New(@isolate)
    key = V8::C::String::NewFromUtf8(@isolate, "key")
    o.Set(@ctx, key, key)
    a = V8::C::Array::New(@isolate)

    a.Set(@ctx, 0, o)

    object = a.CloneElementAt(@ctx, 0)
    expect(object).to be_successful
    expect(object.FromJust().Get(@ctx, key)).to be_successful
    expect(object.FromJust().Get(@ctx, key).FromJust().Utf8Value()).to eql "key"
  end
end
