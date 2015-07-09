require 'c_spec_helper'

describe V8::C::ScriptOrigin do
  requires_v8_context

  describe "with only a name" do
    let(:origin) { V8::C::ScriptOrigin.new V8::C::String::NewFromUtf8(@isolate, "bob.js") }

    it "it hase a resource name" do
      expect(origin.ResourceName().Utf8Value).to eql "bob.js"
    end

    it "has nil for all the other values" do
      expect(origin.ResourceLineOffset()).to be_nil
      expect(origin.ResourceColumnOffset()).to be_nil
      expect(origin.ScriptID()).to be_nil
      expect(origin.SourceMapUrl()).to be_nil
    end
  end

  describe "with all the other options" do
    let(:origin) do
      V8::C::ScriptOrigin.new(
        V8::C::String::NewFromUtf8(@isolate, "bob.js"), 5, 25,
        true, 100, true,
        V8::C::String::NewFromUtf8(@isolate, "http://foo/bob.js.map"),
        false
      )
    end
    it "maps the correct values" do
      expect(origin.ResourceName().Utf8Value()).to eql 'bob.js'
      expect(origin.ResourceLineOffset()).to eql 5
      expect(origin.ResourceColumnOffset()).to eql 25
      expect(origin.ScriptID()).to eql 100
      expect(origin.SourceMapUrl().Utf8Value()).to eql "http://foo/bob.js.map"
    end
  end
end
