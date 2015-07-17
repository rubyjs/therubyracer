require 'c_spec_helper'

describe "V8::C::Number pyramid" do
  requires_v8_context

  describe V8::C::Number do
    let(:number) { V8::C::Number::New @isolate, 25.325 }
    it "sees that value" do
      expect(number.Value()).to eql 25.325
    end
    it "is a kind of primitive" do
      expect(number).to be_kind_of V8::C::Primitive
    end
  end
  describe V8::C::Integer do
    let(:int) { V8::C::Integer::New @isolate, 5 }

    it "is has a value of 5" do
      expect(int.Value()).to eql 5
    end

    it "is a Uint32" do
      expect(int.IsUint32()).to be_truthy
      expect(int).to be_kind_of V8::C::Uint32
    end
  end
  describe V8::C::Int32 do
    let(:int32) { V8::C::Integer::New @isolate, -5 }

    it "has a value of -5" do
      expect(int32.Value()).to eql -5
    end

    it "is an Int32" do
      expect(int32).to be_kind_of V8::C::Int32
    end
  end
end
