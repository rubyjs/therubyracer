require 'c_spec_helper'

describe V8::C::Value do
  requires_v8_context

  describe "Boolean" do
    let(:t) { V8::C::Boolean::New @isolate, true }
    let(:f) { V8::C::Boolean::New @isolate, false }

    it "knows true is true, and false is false" do
      expect(t.IsTrue()).to be true
      expect(t.IsFalse()).to be false
      expect(f.IsTrue()).to be false
      expect(f.IsFalse()).to be true
      expect(t.IsBoolean()).to be true
      expect(f.IsBoolean()).to be true
    end

    it "has a BooleanValue" do
      expect(t.BooleanValue(@ctx).FromJust()).to be true
      expect(f.BooleanValue(@ctx).FromJust()).to be false
    end
  end

  describe "String" do
    let(:string) { V8::C::String::NewFromUtf8(@isolate, "value")}
    it "is a string" do
       expect(string.IsString()).to be true
    end
  end

  describe "Number" do
    let(:int) { V8::C::Integer::New(@isolate, -10) }
    let(:num) { V8::C::Number::New(@isolate, 5.5) }
    let(:uint) { V8::C::Integer::NewFromUnsigned(@isolate, 10) }

    it "recognizes integer types" do
      expect(int.IsNumber).to be true
      expect(int.IsInt32).to be true
      expect(int.IsUint32).to be false
      expect(num.IsNumber()).to be true
      expect(num.IsInt32).to be false
      expect(uint.IsNumber).to be true
      expect(uint.IsUint32).to be true
    end
  end

end
