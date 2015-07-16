require 'c_spec_helper'

describe V8::C::Object do
  requires_v8_context

  it 'can create a new object' do
    expect(V8::C::Object.New(@isolate)).to be
  end

  it 'can store and retrieve a value' do
    o = V8::C::Object.New(@isolate)
    key = V8::C::String.NewFromUtf8(@isolate, 'foo')
    value = V8::C::String.NewFromUtf8(@isolate, 'bar')

    maybe = o.Set(@ctx, key, value)
    expect(maybe.IsJust()).to be true
    expect(maybe.FromJust()).to be true

    maybe = o.Get(@ctx, key)
    expect(maybe.IsJust()).to be true
    expect(maybe.FromJust().Utf8Value).to eq 'bar'
  end

  it 'can determine if a key has been set' do
    o = V8::C::Object.New(@isolate)
    key = V8::C::String.NewFromUtf8(@isolate, 'foo')

    o.Set(@ctx, key, key)

    maybe = o.Has(@ctx, key)
    expect(maybe.IsJust()).to be true
    expect(maybe.FromJust()).to eq true
  end

  it 'can delete keys' do
    o = V8::C::Object.New(@isolate)
    key = V8::C::String.NewFromUtf8(@isolate, 'foo')

    o.Set(@ctx, key, key)

    maybe = o.Delete(@ctx, key)
    expect(maybe.IsJust()).to be true
    expect(maybe.FromJust()).to eq true

    maybe = o.Has(@ctx, key)
    expect(maybe.IsJust()).to be true
    expect(maybe.FromJust()).to eq false
  end

  describe '#SetAccessor' do
    it 'can set getters' do
      o = V8::C::Object.New(@isolate)
      key = V8::C::String.NewFromUtf8(@isolate, 'foo')

      data = V8::C::String.NewFromUtf8(@isolate, 'data')
      get_value = V8::C::String.NewFromUtf8(@isolate, 'bar')

      get_name = nil
      get_data = nil

      getter = proc do |name, info|
        get_name = name
        get_data = info.Data

        info.GetReturnValue.Set(get_value)
      end

      o.SetAccessor(@ctx, key, getter, nil, data)

      maybe = o.Get(@ctx, key)
      expect(maybe.IsJust).to be true
      expect(maybe.FromJust.StrictEquals(get_value)).to be true

      expect(get_name.Equals(key)).to be true
      expect(get_data.StrictEquals(data)).to be true
    end

    it 'can set setters' do
      o = V8::C::Object.New(@isolate)
      key = V8::C::String.NewFromUtf8(@isolate, 'foo')
      data = V8::C::String.NewFromUtf8(@isolate, 'data')

      set_value = nil
      set_data = nil

      setter = proc do |name, value, info|
        set_data = info.Data

        set_value = value
      end

      o.SetAccessor(@ctx, key, proc { }, setter, data)

      maybe = o.Set(@ctx, key, data)
      expect(maybe.IsJust).to be true
      expect(maybe.FromJust).to be true

      expect(set_data.StrictEquals(data)).to be true
      expect(set_value.StrictEquals(data)).to be true
    end
  end

  # TODO: Enable this when the methods are implemented in the extension
  # it 'can retrieve all property names' do
  #   o = V8::C::Object.New
  #   o.Set(V8::C::String.New('foo'), V8::C::String.New('bar'))
  #   o.Set(V8::C::String.New('baz'), V8::C::String.New('bang'))
  #
  #   names = o.GetPropertyNames()
  #   names.Length().should eql 2
  #   names.Get(0).Utf8Value().should eql 'foo'
  #   names.Get(1).Utf8Value().should eql 'baz'
  # end
  #
  # it 'can set an accessor from ruby' do
  #   o = V8::C::Object.New
  #   property = V8::C::String.New('statement')
  #   callback_data = V8::C::String.New('I am Legend')
  #   left = V8::C::String.New('Yo! ')
  #   getter = proc do |name, info|
  #     info.This().StrictEquals(o).should be_true
  #     info.Holder().StrictEquals(o).should be_true
  #     V8::C::String::Concat(left, info.Data())
  #   end
  #   setter = proc do |name, value, info|
  #     left = value
  #   end
  #   o.SetAccessor(property, getter, setter, callback_data)
  #   o.Get(property).Utf8Value().should eql 'Yo! I am Legend'
  #   o.Set(property, V8::C::String::New('Bro! '))
  #   o.Get(property).Utf8Value().should eql 'Bro! I am Legend'
  # end
end
