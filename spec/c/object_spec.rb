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

    expect(o.Set(@ctx, key, value)).to be_successful
    expect(o.Get(@ctx, key)).to strict_eq value
  end

  it 'can determine if a key has been set' do
    o = V8::C::Object.New(@isolate)
    key = V8::C::String.NewFromUtf8(@isolate, 'foo')

    expect(o.Set(@ctx, key, key)).to be_successful
    expect(o.Has(@ctx, key)).to be_successful
  end

  it 'can delete keys' do
    o = V8::C::Object.New(@isolate)
    key = V8::C::String.NewFromUtf8(@isolate, 'foo')

    expect(o.Set(@ctx, key, key)).to be_successful
    expect(o.Delete(@ctx, key)).to be_successful
    expect(o.Has(@ctx, key)).to eq_just false
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

      expect(o.SetAccessor(@ctx, key, getter, nil, data)).to be_successful
      expect(o.Get(@ctx, key)).to strict_eq get_value

      expect(get_name).to v8_eq key
      expect(get_data).to strict_eq data
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

      expect(o.SetAccessor(@ctx, key, proc { }, setter, data)).to be_successful

      expect(o.Set(@ctx, key, data)).to be_successful

      expect(set_data).to strict_eq data
      expect(set_value).to strict_eq data
    end
  end

  describe '#SetAccessorProperty' do
    it 'can set getters' do
      o = V8::C::Object.New(@isolate)
      key = V8::C::String.NewFromUtf8(@isolate, 'foo')

      get_value = V8::C::String.NewFromUtf8(@isolate, 'bar')
      getter = V8::C::Function.New(@isolate, proc { |info| info.GetReturnValue.Set(get_value) })

      o.SetAccessorProperty(@ctx, key, getter)
      expect(o.Get(@ctx, key)).to strict_eq get_value
    end

    it 'can set setters' do
      o = V8::C::Object.New(@isolate)
      key = V8::C::String.NewFromUtf8(@isolate, 'foo')

      set_value = nil

      getter = V8::C::Function.New(@isolate, proc { })
      setter = V8::C::Function.New(@isolate, proc { |info| set_value = info[0] })

      o.SetAccessorProperty(@ctx, key, getter, setter)
      expect(o.Set(@ctx, key, key)).to be_successful
      expect(set_value).to strict_eq key
    end
  end

  describe '#CreateDataProperty' do
    it 'can set the property' do
      o = V8::C::Object.New(@isolate)
      key = V8::C::String.NewFromUtf8(@isolate, 'foo')
      data = V8::C::String.NewFromUtf8(@isolate, 'data')

      expect(o.CreateDataProperty(@ctx, key, data)).to be_successful
      expect(o.Get(@ctx, key)).to be_successful
    end
  end

  describe '#DefineOwnProperty' do
    it 'can set the property' do
      o = V8::C::Object.New(@isolate)
      key = V8::C::String.NewFromUtf8(@isolate, 'foo')
      data = V8::C::String.NewFromUtf8(@isolate, 'data')

      expect(o.DefineOwnProperty(@ctx, key, data)).to be_successful
      expect(o.Get(@ctx, key)).to be_successful
    end
  end

  describe '#GetPropertyAttributes' do
    it 'can get the set attributes' do
      o = V8::C::Object.New(@isolate)
      key = V8::C::String.NewFromUtf8(@isolate, 'foo')
      data = V8::C::String.NewFromUtf8(@isolate, 'data')

      expect(o.DefineOwnProperty(@ctx, key, data, V8::C::PropertyAttribute::DontEnum)).to be_successful
      expect(o.GetPropertyAttributes(@ctx, key)).to eq_just V8::C::PropertyAttribute::DontEnum
    end
  end

  describe '#GetOwnPropertyDescriptor' do
    it 'can read the descriptor of a data property' do
      o = V8::C::Object.New(@isolate)
      key = V8::C::String.NewFromUtf8(@isolate, 'foo')
      data = V8::C::String.NewFromUtf8(@isolate, 'data')

      expect(o.DefineOwnProperty(@ctx, key, data, V8::C::PropertyAttribute::DontEnum)).to be_successful

      maybe = o.GetOwnPropertyDescriptor(@ctx, key)
      expect(maybe).to be_successful

      descriptor = maybe.FromJust
      value = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'value'))
      writable = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'writable'))
      get = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'get'))
      set = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'set'))
      configurable = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'configurable'))
      enumerable = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'enumerable'))

      expect(value).to strict_eq data

      expect(writable).to be_successful
      expect(writable.FromJust.Value).to be true

      expect(get).to be_successful
      expect(get.FromJust).to be_a V8::C::Undefined

      expect(set).to be_successful
      expect(set.FromJust).to be_a V8::C::Undefined

      expect(configurable).to be_successful
      expect(configurable.FromJust.Value).to be true

      expect(enumerable).to be_successful
      expect(enumerable.FromJust.Value).to be false
    end

    it 'can read the descriptor of an accessor property' do
      o = V8::C::Object.New(@isolate)
      key = V8::C::String.NewFromUtf8(@isolate, 'foo')
      data = V8::C::String.NewFromUtf8(@isolate, 'data')

      getter = V8::C::Function.New @isolate, proc { }, V8::C::Object::New(@isolate)
      setter = V8::C::Function.New @isolate, proc { }, V8::C::Object::New(@isolate)

      o.SetAccessorProperty(@ctx, key, getter, setter)

      maybe = o.GetOwnPropertyDescriptor(@ctx, key)
      expect(maybe).to be_successful

      descriptor = maybe.FromJust
      value = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'value'))
      writable = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'writable'))
      get = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'get'))
      set = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'set'))
      configurable = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'configurable'))
      enumerable = descriptor.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'enumerable'))

      expect(value).to be_successful
      expect(value.FromJust).to be_a V8::C::Undefined
      expect(writable).to be_successful

      expect(get).to strict_eq getter
      expect(set).to strict_eq setter

      expect(configurable).to be_successful
      expect(configurable.FromJust.Value).to be true

      expect(enumerable).to be_successful
      expect(enumerable.FromJust.Value).to be true
    end
  end

  describe '#GetPropertyNames' do
    it 'can get array of attribute names' do
      o = V8::C::Object.New(@isolate)
      key_one = V8::C::String.NewFromUtf8(@isolate, 'foo 1')
      key_two = V8::C::String.NewFromUtf8(@isolate, 'foo 2')
      data = V8::C::String.NewFromUtf8(@isolate, 'data')

      expect(o.DefineOwnProperty(@ctx, key_one, data)).to be_successful
      expect(o.DefineOwnProperty(@ctx, key_two, data)).to be_successful

      names = o.GetPropertyNames(@ctx)
      expect(names).to be_successful

      expect(names.FromJust.Get(@ctx, 0)).to v8_eq key_one
      expect(names.FromJust.Get(@ctx, 1)).to v8_eq key_two
    end
  end

  describe '#GetOwnPropertyNames' do
    it 'can get array of attribute names' do
      o = V8::C::Object.New(@isolate)
      key_one = V8::C::String.NewFromUtf8(@isolate, 'foo 1')
      key_two = V8::C::String.NewFromUtf8(@isolate, 'foo 2')
      data = V8::C::String.NewFromUtf8(@isolate, 'data')

      expect(o.DefineOwnProperty(@ctx, key_one, data)).to be_successful
      expect(o.DefineOwnProperty(@ctx, key_two, data)).to be_successful

      names = o.GetOwnPropertyNames(@ctx)
      expect(names).to be_successful

      expect(names.FromJust.Get(@ctx, 0)).to v8_eq key_one
      expect(names.FromJust.Get(@ctx, 1)).to v8_eq key_two
    end
  end

  context 'with prototypes' do
    let(:o) { V8::C::Object.New(@isolate) }
    let(:prototype) { V8::C::Object.New(@isolate) }
    let(:prototype_key) { V8::C::String.NewFromUtf8(@isolate, 'foo') }
    let(:o_key) { V8::C::String.NewFromUtf8(@isolate, 'bar') }

    before do
      expect(prototype.Set(@ctx, prototype_key, prototype_key)).to be_successful
      expect(o.Set(@ctx, o_key, o_key)).to be_successful

      expect(o.SetPrototype(@ctx, prototype)).to be_successful
    end

    it 'can set the object prototype' do
      expect(o.Get(@ctx, prototype_key)).to strict_eq prototype_key
    end

    it 'can enumerate only own properties' do
      names = o.GetOwnPropertyNames(@ctx)

      expect(names).to be_successful
      expect(names.FromJust.Get(@ctx, 0)).to v8_eq o_key

      has_own = o.HasOwnProperty(@ctx, o_key)
      expect(has_own.IsJust).to be true
      expect(has_own.FromJust).to be true

      has_own = o.HasOwnProperty(@ctx, prototype_key)
      expect(has_own.IsJust).to be true
      expect(has_own.FromJust).to be false
    end

    it 'can enumerate all properties' do
      names = o.GetPropertyNames(@ctx)

      expect(names).to be_successful
      expect(names.FromJust.Get(@ctx, 0)).to v8_eq o_key
      expect(names.FromJust.Get(@ctx, 1)).to v8_eq prototype_key
    end
  end

  describe '#ObjectProtoToString' do
    it 'can return a string representation of simple objects' do
      o = V8::C::Object.New(@isolate)
      str = o.ObjectProtoToString(@ctx)

      expect(str).to be_successful
      expect(str.FromJust.Utf8Value).to eq '[object Object]'
    end

    it 'can return a string representation of simple objects' do
      a = V8::C::Array.New(@isolate)

      str = a.ObjectProtoToString(@ctx)
      expect(str).to be_successful
      expect(str.FromJust.Utf8Value).to eq '[object Array]'
    end
  end

  describe '#GetConstructorName' do
    it 'can return the constructor name of an array' do
      a = V8::C::Array.New(@isolate)

      expect(a.GetConstructorName().Utf8Value).to eq 'Array'
    end
  end

  context 'with internal fields' do
    let(:o) do
      template = V8::C::ObjectTemplate.New(@isolate)
      template.SetInternalFieldCount(2)

      template.NewInstance(@ctx).FromJust
    end

    let(:value_one) { V8::C::String.NewFromUtf8(@isolate, 'value 1') }
    let(:value_two) { V8::C::String.NewFromUtf8(@isolate, 'value 2') }

    before do
      o.SetInternalField(0, value_one)
      o.SetInternalField(1, value_two)
    end

    it 'can get and set internal fields' do
      expect(o.GetInternalField(0)).to strict_eq value_one
      expect(o.GetInternalField(1)).to strict_eq value_two
    end

    it 'can get the internal field count' do
      expect(o.InternalFieldCount).to eq 2
    end
  end

  context 'with hidden values' do
    let(:o) { V8::C::Object.New @isolate }

    let(:key_one) { V8::C::String.NewFromUtf8(@isolate, 'key 1') }
    let(:key_two) { V8::C::String.NewFromUtf8(@isolate, 'key 2') }
    let(:value_one) { V8::C::String.NewFromUtf8(@isolate, 'value 1') }
    let(:value_two) { V8::C::String.NewFromUtf8(@isolate, 'value 2') }

    before do
      o.SetHiddenValue(key_one, value_one)
      o.SetHiddenValue(key_two, value_two)
    end

    it 'can get and set hidden values' do
      expect(o.GetHiddenValue(key_one)).to strict_eq value_one
      expect(o.GetHiddenValue(key_two)).to strict_eq value_two
    end

    it 'can delete hidden values' do
      expect(o.DeleteHiddenValue(key_one)).to be true
      expect(o.GetHiddenValue(key_one)).to be nil
    end
  end

  describe '#Clone' do
    let(:o) { V8::C::Object.New @isolate }

    let(:key) { V8::C::String.NewFromUtf8(@isolate, 'key') }
    let(:value) { V8::C::String.NewFromUtf8(@isolate, 'value') }

    before do
      o.Set(@ctx, key, value)
    end

    it 'can clone an object' do
      clone = o.Clone

      expect(clone).to_not strict_eq o

      expect(clone.Get(@ctx, key)).to strict_eq value
    end
  end

  describe '#CreationContext' do
    it 'returns a context' do
      o = V8::C::Object.New @isolate
      expect(o.CreationContext).to be_a V8::C::Context
    end
  end

  describe '#IsCallable' do
    it 'returns false for plain objects' do
      o = V8::C::Object.New @isolate
      expect(o.IsCallable).to be false
    end

    it 'returns true for functions' do
      fn = V8::C::Function.New @isolate, proc { }, V8::C::Object::New(@isolate)
      expect(fn.IsCallable).to be true
    end
  end

  describe 'callable objects' do
    let(:value) { V8::C::String.NewFromUtf8(@isolate, 'value') }
    let(:one) { V8::C::String.NewFromUtf8(@isolate, 'one') }
    let(:two) { V8::C::String.NewFromUtf8(@isolate, 'two') }

    let(:fn) do
      source = '(function(one, two) {this.one = one; this.two = two; return this;})'
      source = V8::C::String.NewFromUtf8(@isolate, source.to_s)
      script = V8::C::Script.Compile(@ctx, source)
      result = script.FromJust.Run(@ctx)

      result.FromJust
    end

    it 'can be called as functions' do
      object = V8::C::Object::New(@isolate)
      result = fn.CallAsFunction(@ctx, object, [one, two]).FromJust

      expect(result).to strict_eq object
      expect(object.Get(@ctx, one)).to strict_eq one
      expect(object.Get(@ctx, two)).to strict_eq two
    end

    it 'can be called as constructors' do
      result = fn.CallAsConstructor(@ctx, [one, two]).FromJust

      expect(result).to be_a V8::C::Object
      expect(result.Get(@ctx, one)).to strict_eq one
      expect(result.Get(@ctx, two)).to strict_eq two
    end
  end
end
