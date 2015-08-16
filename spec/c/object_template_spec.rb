require 'c_spec_helper'

describe V8::C::ObjectTemplate do
  requires_v8_context
  let(:template) { V8::C::ObjectTemplate::New(@isolate) }

  it "can be used to create object instances" do
    expect(template.NewInstance(@ctx).FromJust()).to be_instance_of V8::C::Object
  end

  describe 'InternalFieldCount' do
    it 'can get and set internal field count' do
      template.SetInternalFieldCount(10);
      expect(template.InternalFieldCount).to eq 10
    end
  end

  describe 'SetAccessor' do
    it 'can set getters' do
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
      template.SetAccessor(key, getter, nil, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.Get(@ctx, key)).to strict_eq get_value

      expect(get_name).to v8_eq key
      expect(get_data).to strict_eq data
    end

    it 'can set setters' do
      key = V8::C::String.NewFromUtf8(@isolate, 'foo')
      data = V8::C::String.NewFromUtf8(@isolate, 'data')

      set_value = nil
      set_data = nil

      setter = proc do |name, value, info|
        set_data = info.Data
        set_value = value
      end

      template.SetAccessor(key, proc { }, setter, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.Set(@ctx, key, data)).to be_successful

      expect(set_data).to strict_eq data
      expect(set_value).to strict_eq data
    end
  end

  describe 'SetNamedPropertyHandler' do
    let(:key) { V8::C::String.NewFromUtf8(@isolate, 'foo') }
    let(:data) { V8::C::String.NewFromUtf8(@isolate, 'data') }

    it 'can set getters' do
      get_value = V8::C::String.NewFromUtf8(@isolate, 'bar')

      get_name = nil
      get_data = nil

      getter = proc do |name, info|
        get_name = name
        get_data = info.Data

        info.GetReturnValue.Set(get_value)
      end
      template.SetNamedPropertyHandler(getter, nil, nil, nil, nil, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.Get(@ctx, key)).to strict_eq get_value

      expect(get_name).to v8_eq key
      expect(get_data).to strict_eq data
    end

    it 'can set setters' do
      set_value = nil
      set_data = nil

      setter = proc do |name, value, info|
        set_data = info.Data
        set_value = value
      end

      template.SetNamedPropertyHandler(proc { }, setter, nil, nil, nil, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.Set(@ctx, key, data)).to be_successful

      expect(set_data).to strict_eq data
      expect(set_value).to strict_eq data
    end

    it 'can set query callbacks' do
      value = V8::C::PropertyAttribute::ReadOnly

      query_name = nil
      query_data = nil

      query = proc do |name, info|
        query_name = name
        query_data = info.Data

        info.GetReturnValue.Set_int32_t(value)
      end
      template.SetNamedPropertyHandler(proc { }, nil, query, nil, nil, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.GetPropertyAttributes(@ctx, key).FromJust).to eq value

      expect(query_name).to v8_eq key
      expect(query_data).to strict_eq data
    end

    it 'can set deleters' do
      deleter_name = nil
      deleter_data = nil

      deleter = proc do |name, info|
        deleter_name = name
        deleter_data = info.Data

        info.GetReturnValue.Set_bool(false)
      end
      template.SetNamedPropertyHandler(proc { }, nil, nil, deleter, nil, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.Delete(@ctx, key)).to_not be_successful

      expect(deleter_name).to v8_eq key
      expect(deleter_data).to strict_eq data
    end

    it 'can set enumerators' do
      array = V8::C::Array::New(@isolate)
      array.Set(@ctx, 0, key)

      enumerator_data = nil

      enumerator = proc do |info|
        enumerator_data = info.Data

        info.GetReturnValue.Set(array)
      end
      template.SetNamedPropertyHandler(proc { }, nil, nil, nil, enumerator, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.GetPropertyNames(@ctx).FromJust.Get(@ctx, 0)).to v8_eq key

      expect(enumerator_data).to strict_eq data
    end
  end

  describe 'SetIndexedPropertyHandler' do
    let(:key) { V8::C::Integer.New(@isolate, 1) }
    let(:data) { V8::C::String.NewFromUtf8(@isolate, 'data') }

    it 'can set getters' do
      get_value = V8::C::String.NewFromUtf8(@isolate, 'bar')

      get_index = nil
      get_data = nil

      getter = proc do |index, info|
        get_index = index
        get_data = info.Data

        info.GetReturnValue.Set(get_value)
      end
      template.SetIndexedPropertyHandler(getter, nil, nil, nil, nil, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.Get(@ctx, key)).to strict_eq get_value

      expect(get_index).to eq 1
      expect(get_data).to strict_eq data
    end

    it 'can set setters' do
      set_value = nil
      set_data = nil

      setter = proc do |index, value, info|
        set_data = info.Data
        set_value = value
      end

      template.SetIndexedPropertyHandler(proc { }, setter, nil, nil, nil, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.Set(@ctx, key, data)).to be_successful

      expect(set_data).to strict_eq data
      expect(set_value).to strict_eq data
    end

    it 'can set query callbacks' do
      value = V8::C::PropertyAttribute::ReadOnly

      query_index = nil
      query_data = nil

      query = proc do |index, info|
        query_index = index
        query_data = info.Data

        info.GetReturnValue.Set_int32_t(value)
      end
      template.SetIndexedPropertyHandler(proc { }, nil, query, nil, nil, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.GetPropertyAttributes(@ctx, key).FromJust).to eq value

      expect(query_index).to eq 1
      expect(query_data).to strict_eq data
    end

    it 'can set deleters' do
      deleter_index = nil
      deleter_data = nil

      deleter = proc do |index, info|
        deleter_index = index
        deleter_data = info.Data

        info.GetReturnValue.Set_bool(false)
      end
      template.SetIndexedPropertyHandler(proc { }, nil, nil, deleter, nil, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.Delete(@ctx, key)).to_not be_successful

      expect(deleter_index).to eq 1
      expect(deleter_data).to strict_eq data
    end

    it 'can set enumerators' do
      array = V8::C::Array::New(@isolate)
      array.Set(@ctx, 0, key)

      enumerator_data = nil

      enumerator = proc do |info|
        enumerator_data = info.Data

        info.GetReturnValue.Set(array)
      end
      template.SetIndexedPropertyHandler(proc { }, nil, nil, nil, enumerator, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.GetPropertyNames(@ctx).FromJust.Get(@ctx, 0)).to v8_eq key

      expect(enumerator_data).to strict_eq data
    end
  end

  describe 'SetCallAsFunctionHandler' do
    let(:data) { V8::C::String.NewFromUtf8(@isolate, 'data') }

    it 'can set a CallAsFunction handler' do
      callback_data = nil

      callback = proc do |info|
        callback_data = info.Data

        info.GetReturnValue.Set(data)
      end

      template.SetCallAsFunctionHandler(callback, data)

      o = template.NewInstance(@ctx).FromJust
      expect(o.CallAsFunction(@ctx, o, [])).to strict_eq data
    end
  end
end
