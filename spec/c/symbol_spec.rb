require 'c_spec_helper'

describe V8::C::Symbol do
  requires_v8_context

  describe "without a description" do
    let(:symbol) { V8::C::Symbol::New(@isolate)}

    it "exists" do
      expect(symbol).to be
    end

    it "has an identity hash" do
      expect(symbol.GetIdentityHash()).to be
    end
  end

  describe "with a description" do
    let(:description) { V8::C::String::NewFromUtf8(@isolate, "bob") }
    let(:symbol) { V8::C::Symbol::New(@isolate, description) }

    it "has a name" do
      expect(symbol.Name().Utf8Value()).to eql "bob"
    end
  end

  describe "from symbol registries" do
    let(:key) { V8::C::String::NewFromUtf8(@isolate, "mysym") }
    let(:global) { V8::C::Symbol::For(@isolate, key) }
    let(:api) { V8::C::Symbol::ForApi(@isolate, key) }

    it "always retrieves the same value for a given key" do
      expect(V8::C::Symbol::For(@isolate, key).StrictEquals(global)).to be true
      expect(V8::C::Symbol::ForApi(@isolate, key).StrictEquals(api)).to be true
    end

    it "returns different symbols for different registries" do
      expect(global).to_not strict_eq api
    end
  end

  describe "well-known symbols" do
    it "GetIterator" do
      expect(V8::C::Symbol::GetIterator(@isolate)).to be_kind_of V8::C::Symbol
    end
    it "GetUnscopables" do
      expect(V8::C::Symbol::GetUnscopables(@isolate)).to be_kind_of V8::C::Symbol
    end
    it "GetToStringTag" do
      expect(V8::C::Symbol::GetToStringTag(@isolate)).to be_kind_of V8::C::Symbol
    end
  end
end
