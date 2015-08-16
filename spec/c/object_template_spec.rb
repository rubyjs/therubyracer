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
end
